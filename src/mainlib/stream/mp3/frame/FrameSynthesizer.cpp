#include "FrameSynthesizer.h"

#include <mainlib/math/types.h>

#include <algorithm>
#include <cmath>

using namespace aixa::math;

FrameSynthesizer::GranulesDequantized FrameSynthesizer::buildGranulesDequantized() {
    ChannelsDequantized template_ = {aixa::math::DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS),
                                     aixa::math::DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS)};
    return {template_, template_};
}

FrameSynthesizer::FrameSynthesizer(std::unique_ptr<BlockSynthesisAlgorithms> longWindowSFComputer,
                                   std::unique_ptr<BlockSynthesisAlgorithms> shortWindowSFComputer,
                                   AntialiasCoefficients antialiasCoefficients,
                                   BlockWindows blockWindows,
                                   aixa::math::DoubleMatrix frequencyInversion,
                                   aixa::math::DoubleMatrix synFilter,
                                   aixa::math::DoubleMatrix dWindow) :
    longWindowAlgorithms(std::move(longWindowSFComputer)),
    shortWindowAlgorithms(std::move(shortWindowSFComputer)), antialiasCoefficients(antialiasCoefficients),
    blockWindows(std::move(blockWindows)), frequencyInversion(std::move(frequencyInversion)),
    synthesisFilter(std::move(synFilter)), dWindow(std::move(dWindow)),
    granulesDequantized(buildGranulesDequantized()), timeSamples(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS),
    overlappingTerms() {
    resetFIFOs();
}

FrameSamples FrameSynthesizer::synthesize(const Frame& frame) {
    auto ret = FrameSamples();

    for (unsigned int i = 0; i < NR_GRANULES; i++) {
        for (unsigned int channel = 0; channel < frame.header.channels(); channel++) {
            const auto& channelInfo = frame.sideInfo.granules[i][channel];
            const auto& channelContent = frame.content.granules[i][channel];

            granulesDequantized[i][channel] =
                dequantizeSamples(frame.header.samplingFreq, channelInfo, channelContent);
        }
        jointStereo(frame.header, granulesDequantized[i]);
    }

    auto samples = &ret.channel1;
    for (unsigned int channel = 0; channel < frame.header.channels(); channel++) {
        std::size_t startIndex = 0;
        for (unsigned int i = 0; i < NR_GRANULES; i++) {
            const auto& channelInfo = frame.sideInfo.granules[i][channel];

            synthesizeGranuleChannel(*samples, granulesDequantized[i][channel], overlappingTerms[channel],
                                     fifoOfChannel[channel], frame.header, channelInfo, startIndex);

            startIndex = NR_GRANULE_SAMPLES;
        }
        samples = &ret.channel2;
    }

    return ret;
}

aixa::math::DoubleMatrix FrameSynthesizer::dequantizeSamples(unsigned int samplingFreq,
                                                             const GranuleChannelSideInfo& channelInfo,
                                                             const GranuleChannelContent& channelContent) {
    auto ret = aixa::math::DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS);
    auto& scaleFactorsComputer = (channelInfo.blockType != GranuleChannelSideInfo::BlockType::THREE_SHORT)
                                     ? *longWindowAlgorithms
                                     : *shortWindowAlgorithms;

    auto windowScaleFactors =
        scaleFactorsComputer.computeScaleFactors(samplingFreq, channelInfo, channelContent);
    double gainTerm = std::pow(2.0, (channelInfo.globalGain - GAIN_BASE) / 4.0);

    for (std::size_t band = 0; band < NR_FREQ_BANDS; band++) {
        for (std::size_t sampleInd = 0; sampleInd < NR_CODED_SAMPLES_PER_BAND; sampleInd++) {
            double scaleFactorTerm = windowScaleFactors[band][sampleInd];
            auto sample = channelContent.freqBands[band][sampleInd];
            auto dequantizedSample = std::pow(std::abs(sample), 4.0 / 3.0) * gainTerm * scaleFactorTerm;
            dequantizedSample *= (sample < 0) ? -1 : 1;
            ret(band, sampleInd) = dequantizedSample;
        }
    }

    return ret;
}

void FrameSynthesizer::jointStereo(const FrameHeader& header, ChannelsDequantized& channelsDequantized) {
    if (header.mode != FrameHeader::Mode::JOINT_STEREO)
        return;

    const auto& firstChannel = channelsDequantized[0];
    const auto& secondChannel = channelsDequantized[1];

    std::array<aixa::math::DoubleMatrix, 2> temp = {
        aixa::math::DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS),
        aixa::math::DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS)};

    for (std::size_t row = 0; row < NR_FREQ_BANDS; row++) {
        for (std::size_t col = 0; col < NR_CODED_SAMPLES_PER_BAND; col++) {
            double firstChannelSample, secondChannelSample;
            if (header.msStereo) {
                firstChannelSample = (firstChannel(row, col) + secondChannel(row, col)) / 1.41421356;
                secondChannelSample = (firstChannel(row, col) - secondChannel(row, col)) / 1.41421356;
            } else {
                firstChannelSample = firstChannel(row, col);
                secondChannelSample = secondChannel(row, col);
            }
            temp[0](row, col) = firstChannelSample;
            temp[1](row, col) = secondChannelSample;
        }
    }

    channelsDequantized = std::move(temp);
}

void FrameSynthesizer::synthesizeGranuleChannel(ChannelSamples& samples,
                                                aixa::math::DoubleMatrix& dequantized,
                                                Bands<double>& overlapping,
                                                ChannelFifo& fifo,
                                                const FrameHeader& header,
                                                const GranuleChannelSideInfo& channelInfo,
                                                std::size_t startIndex) {
    reorder(dequantized, header.samplingFreq, channelInfo);

    antialias(dequantized, channelInfo);

    inverseMDCT(dequantized, channelInfo, overlapping);

    polyphaseSynthesis(samples, fifo, startIndex);
}

void FrameSynthesizer::reorder(aixa::math::DoubleMatrix& dequantized,
                               unsigned int samplingFreq,
                               const GranuleChannelSideInfo& channelInfo) {
    if (channelInfo.blockType != GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        return;
    }

    std::size_t band = 0;
    auto temp = aixa::math::DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS);

    if (channelInfo.mixedBlockFlag) {
        for (; band < 2; band++) {
            for (std::size_t col = 0; col < NR_CODED_SAMPLES_PER_BAND; col++) {
                temp(band, col) = dequantized(band, col);
            }
        }
        band++;
    }

    for (; band < NR_SHORT_WINDOW_RANGES; band++) {
        for (std::size_t window = 0; window < NR_SHORT_WINDOWS; window++) {
            auto startLine = samplingFreqBandIndexes[samplingFreq].shortWindow[band];
            auto finalLine = samplingFreqBandIndexes[samplingFreq].shortWindow[band + 1];
            auto linesCount = finalLine - startLine;
            for (std::size_t freqLine = 0; freqLine < linesCount; freqLine++) {
                auto srcLine = (startLine * NR_SHORT_WINDOWS) + (window * linesCount) + freqLine;
                auto dstLine = (startLine * NR_SHORT_WINDOWS) + window + (freqLine * NR_SHORT_WINDOWS);
                temp(dstLine / NR_CODED_SAMPLES_PER_BAND, dstLine % NR_CODED_SAMPLES_PER_BAND) =
                    dequantized(srcLine / NR_CODED_SAMPLES_PER_BAND, srcLine % NR_CODED_SAMPLES_PER_BAND);
            }
        }
    }
    dequantized = std::move(temp);
}

void FrameSynthesizer::antialias(aixa::math::DoubleMatrix& dequantized,
                                 const GranuleChannelSideInfo& channelInfo) {
    std::size_t subbands = NR_FREQ_BANDS - 1;

    if (channelInfo.windowSwitching &&
        channelInfo.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        if (channelInfo.mixedBlockFlag) {
            subbands = 1;
        } else {
            return;
        }
    }

    for (std::size_t band = 0; band < subbands; band++) {
        for (std::size_t butterfly = 0; butterfly < NR_BUTTERFLIES; butterfly++) {
            auto upIndex = NR_CODED_SAMPLES_PER_BAND - 1 - butterfly;
            auto bandUp = dequantized(band, upIndex);
            auto bandDown = dequantized(band + 1, butterfly);
            auto cs = antialiasCoefficients.cs[butterfly];
            auto ca = antialiasCoefficients.ca[butterfly];
            dequantized(band, upIndex) = cs * bandUp - ca * bandDown;
            dequantized(band + 1, butterfly) = ca * bandUp + cs * bandDown;
        }
    }
}

void FrameSynthesizer::inverseMDCT(const aixa::math::DoubleMatrix& dequantized,
                                   const GranuleChannelSideInfo& info,
                                   Bands<double>& overlappingTerms) {
    const auto& window = blockWindows.at(info.blockType);
    auto& transformComputer = (info.blockType != GranuleChannelSideInfo::BlockType::THREE_SHORT)
                                  ? *longWindowAlgorithms
                                  : *shortWindowAlgorithms;
    auto overlappedTimeSamples = transformComputer.computeInverseMDCT(dequantized, window);

    for (std::size_t bandInd = 0; bandInd < NR_FREQ_BANDS; bandInd++) {
        const auto bandSamples = overlappedTimeSamples.row(bandInd);
        auto& bandOverlapping = overlappingTerms[bandInd];
        for (std::size_t sample = 0; sample < NR_CODED_SAMPLES_PER_BAND; sample++) {
            timeSamples(bandInd, sample) = bandSamples[sample] + bandOverlapping[sample];
            bandOverlapping[sample] = bandSamples[sample + NR_CODED_SAMPLES_PER_BAND];
        }
    }

    timeSamples.elemWiseProduct(frequencyInversion);
}

void FrameSynthesizer::polyphaseSynthesis(ChannelSamples& samples,
                                          ChannelFifo& fifo,
                                          std::size_t startIndex) {
    auto buildMatrix = [&fifo]() {
        auto ret = DoubleMatrix(D_WINDOW_VECTORS, NR_D_WINDOW_VECTOR_SIZE);
        for (std::size_t row = 0; row < NR_D_WINDOW_VECTOR_SIZE; row++) {
            for (std::size_t col = 0; col < D_WINDOW_VECTORS; col++) {
                auto startOffset = (col % 2) ? NR_D_WINDOW_VECTOR_SIZE : 0;
                ret(row, col) = fifo[col][startOffset + row];
            }
        }

        return ret;
    };

    auto clip = [](DoubleMatrix& result) {
        std::for_each(result.content().begin(), result.content().end(), [](double& d) {
            if (d >= MAX_DECODED_VALUE) {
                d = MAX_DECODED_VALUE - 1;
            } else if (d < -MAX_DECODED_VALUE) {
                d = -MAX_DECODED_VALUE;
            }
        });
    };

    auto matrixed = timeSamples.transpose() * synthesisFilter;

    for (std::size_t i = 0; i < NR_PCM_BLOCKS; i++) {
        auto row = matrixed.row(i);
        fifo.pop_back();
        fifo.emplace_front(row.begin(), row.size());
        auto matrix = buildMatrix();
        auto result = SCALE * (dWindow.elemWiseProduct(matrix).collapseRows());
        clip(result);
        std::size_t samplesOffset = startIndex + (i * NR_BLOCK_SAMPLES);
        std::copy(result.content().begin(), result.content().end(), samples.begin() + samplesOffset);
    }
}

void FrameSynthesizer::clearState() { resetFIFOs(); }

void FrameSynthesizer::resetFIFOs() {
    for (auto& fifo : fifoOfChannel) {
        fifo.clear();

        for (std::size_t i = 0; i < D_WINDOW_VECTORS; i++) {
            fifo.emplace_front(NR_D_WINDOW_MATRIXED_VECTOR_SIZE);
        }
    }
}
