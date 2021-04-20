#include "FrameSynthesizer.h"

#include <mainlib/math/types.h>

#include <cmath>

using namespace aixa::math;

FrameSynthesizer::FrameSynthesizer(std::unique_ptr<WindowScaleFactorsComputer> longWindowSFComputer,
                                   std::unique_ptr<WindowScaleFactorsComputer> shortWindowSFComputer,
                                   AntialiasCoefficients antialiasCoefficients,
                                   aixa::math::DoubleMatrix cosineTransform,
                                   BlockWindows blockWindows,
                                   aixa::math::DoubleMatrix frequencyInversion,
                                   aixa::math::DoubleMatrix synFilter,
                                   aixa::math::DoubleMatrix dWindow) :
    longWindowSFComputer(std::move(longWindowSFComputer)),
    shortWindowSFComputer(std::move(shortWindowSFComputer)), antialiasCoefficients(antialiasCoefficients),
    cosineTransform(std::move(cosineTransform)), blockWindows(std::move(blockWindows)),
    frequencyInversion(std::move(frequencyInversion)), synthesisFilter(std::move(synFilter)),
    dWindow(std::move(dWindow)), dequantized(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS),
    timeSamples(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS), channelOverlappingTerms() {
    resetFIFO();
}

FrameSamples FrameSynthesizer::synthesize(const Frame& frame) {
    auto ret = FrameSamples();
    auto samples = &ret.channel1;
    for (unsigned int channel = 0; channel < frame.header.channels(); channel++) {
        std::size_t startIndex = 0;
        for (unsigned int i = 0; i < NR_GRANULES; i++) {
            const auto& channelInfo = frame.sideInfo.granules[i][channel];
            const auto& channelContent = frame.content.granules[i][channel];

            synthesizeGranuleChannel(*samples, channel, frame.header.samplingFreq, channelInfo,
                                     channelContent, startIndex);

            startIndex = NR_GRANULE_SAMPLES;
        }

        samples = &ret.channel2;
    }

    return ret;
}

void FrameSynthesizer::synthesizeGranuleChannel(ChannelSamples& samples,
                                                unsigned int channel,
                                                unsigned int samplingFreq,
                                                const GranuleChannelSideInfo& channelInfo,
                                                const GranuleChannelContent& channelContent,
                                                std::size_t startIndex) {
    dequantizeSamples(samplingFreq, channelInfo, channelContent);

    reorder(samplingFreq, channelInfo, channelContent);

    // stereo

    antialias(channelInfo);

    inverseMDCT(channelInfo, channelOverlappingTerms[channel]);

    polyphaseSynthesis(samples, startIndex);
}

void FrameSynthesizer::dequantizeSamples(unsigned int samplingFreq,
                                         const GranuleChannelSideInfo& channelInfo,
                                         const GranuleChannelContent& channelContent) {
    auto& scaleFactorsComputer = (channelInfo.blockType != GranuleChannelSideInfo::BlockType::THREE_SHORT)
                                     ? *longWindowSFComputer
                                     : *shortWindowSFComputer;

    auto windowScaleFactors = scaleFactorsComputer.compute(samplingFreq, channelInfo, channelContent);
    double gainTerm = std::pow(2.0, (channelInfo.globalGain - GAIN_BASE) / 4.0);

    for (std::size_t band = 0; band < NR_FREQ_BANDS; band++) {
        for (std::size_t sampleInd = 0; sampleInd < NR_CODED_SAMPLES_PER_BAND; sampleInd++) {
            double scaleFactorTerm = windowScaleFactors[band][sampleInd];
            auto sample = channelContent.freqBands[band][sampleInd];
            auto dequantizedSample = std::pow(std::abs(sample), 4.0 / 3.0) * gainTerm * scaleFactorTerm;
            dequantizedSample *= (sample < 0) ? -1 : 1;
            dequantized(band, sampleInd) = dequantizedSample;
        }
    }
}

void FrameSynthesizer::reorder(unsigned int samplingFreq,
                               const GranuleChannelSideInfo& channelInfo,
                               const GranuleChannelContent& channelContent) {
    if (channelInfo.blockType != GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        return;
    }

    if (channelInfo.mixedBlockFlag) {
    } else {
        auto temp = aixa::math::DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS);

        for (std::size_t band = 0; band < NR_SHORT_WINDOW_BANDS; band++) {
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
}

void FrameSynthesizer::antialias(const GranuleChannelSideInfo& channelInfo) {
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

void FrameSynthesizer::inverseMDCT(const GranuleChannelSideInfo& info, Bands<double>& overlappingTerms) {
    const auto& window = blockWindows.at(info.blockType);
    auto overlappedTimeSamples = dequantized * cosineTransform * window;

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

void FrameSynthesizer::polyphaseSynthesis(ChannelSamples& samples, std::size_t startIndex) {
    auto buildMatrix = [&]() {
        auto ret = DoubleMatrix(D_WINDOW_VECTORS, NR_D_WINDOW_VECTOR_SIZE);
        for (std::size_t row = 0; row < NR_D_WINDOW_VECTOR_SIZE; row++) {
            for (std::size_t col = 0; col < D_WINDOW_VECTORS; col++) {
                auto startOffset = (col % 2) ? NR_D_WINDOW_VECTOR_SIZE : 0;
                ret(row, col) = fifo[col][startOffset + row];
            }
        }

        return ret;
    };

    auto matrixed = timeSamples.transpose() * synthesisFilter;

    for (std::size_t i = 0; i < NR_PCM_BLOCKS; i++) {
        auto row = matrixed.row(i);
        fifo.pop_back();
        fifo.emplace_front(row.begin(), row.size());
        auto matrix = buildMatrix();
        auto result = SCALE * (dWindow.elemWiseProduct(matrix).collapseRows());
        std::size_t samplesOffset = startIndex + (i * NR_BLOCK_SAMPLES);
        std::copy(result.constContent().begin(), result.constContent().end(),
                  samples.begin() + samplesOffset);
    }
}

void FrameSynthesizer::clearState() { resetFIFO(); }

void FrameSynthesizer::resetFIFO() {
    fifo.clear();

    for (std::size_t i = 0; i < D_WINDOW_VECTORS; i++) {
        fifo.emplace_front(NR_D_WINDOW_MATRIXED_VECTOR_SIZE);
    }
}