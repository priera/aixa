#include "FrameSynthesizer.h"

#include <mainlib/math/types.h>

#include <cmath>
#include <stdexcept>

using namespace aixa::math;

std::vector<unsigned int> FrameSynthesizer::pretab = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0};

FrameSynthesizer::FrameSynthesizer(AntialiasCoefficients antialiasCoefficients,
                                   aixa::math::DoubleMatrix cosineTransform,
                                   BlockWindows blockWindows,
                                   aixa::math::DoubleMatrix frequencyInversion,
                                   aixa::math::DoubleMatrix synFilter,
                                   aixa::math::DoubleMatrix dWindow) :
    antialiasCoefficients(antialiasCoefficients),
    cosineTransform(std::move(cosineTransform)), blockWindows(std::move(blockWindows)),
    frequencyInversion(std::move(frequencyInversion)), synthesisFilter(std::move(synFilter)),
    dWindow(std::move(dWindow)), dequantized(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS),
    timeSamples(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS), channelOverlappingTerms() {
    for (std::size_t i = 0; i < 16; i++) {
        fifo.emplace_front(64);
    }
}

void FrameSynthesizer::synthesize(unsigned int samplingFreq,
                                  const SideInformation& sideInfo,
                                  const MainDataContent& content,
                                  std::size_t nChannels) {
    for (unsigned int i = 0; i < NR_GRANULES; i++) {
        for (unsigned int channel = 0; channel < nChannels; channel++) {
            const auto& channelInfo = sideInfo.granules[i][channel];
            const auto& channelContent = content.granules[i][channel];
            dequantizeSamples(samplingFreq, channelInfo, channelContent);
            // reordering (short windows only)
            // stereo
            antialias(channelInfo);
            inverseMDCT(channelInfo, channelOverlappingTerms[channel]);
            polyphaseSynthesis();
        }
    }
}

void FrameSynthesizer::dequantizeSamples(unsigned int samplingFreq,
                                         const GranuleChannelSideInfo& channelInfo,
                                         const GranuleChannelContent& channelContent) {
    if (channelInfo.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        throw std::runtime_error("Not supported (yet) short windows");
    }

    double gainTerm = std::pow(2.0, (channelInfo.globalGain - GAIN_BASE) / 4.0);
    std::size_t scaleFactorBandInd = 1;
    std::size_t nextSubbandBoundary = samplingFreqBandIndexes[samplingFreq].longWindow[scaleFactorBandInd];

    for (std::size_t band = 0; band < NR_FREQ_BANDS; band++) {
        for (std::size_t sampleInd = 0; sampleInd < NR_CODED_SAMPLES_PER_BAND; sampleInd++) {
            if ((band * NR_CODED_SAMPLES_PER_BAND) + sampleInd == nextSubbandBoundary) {
                scaleFactorBandInd++;
                nextSubbandBoundary = samplingFreqBandIndexes[samplingFreq].longWindow[scaleFactorBandInd];
            }

            auto scaleFactor = channelContent.longWindowScaleFactorBands[scaleFactorBandInd - 1];
            double preTabFactor = channelInfo.preFlag * pretab[scaleFactorBandInd - 1];
            double exp = -0.5 * (1.0 + channelInfo.scaleFactorScale) * (scaleFactor + preTabFactor);
            double scaleFactorTerm = std::pow(2.0, exp);
            auto sample = channelContent.freqBands[band][sampleInd];
            auto dequantizedSample = std::pow(std::abs(sample), 4.0 / 3.0) * gainTerm * scaleFactorTerm;
            dequantizedSample *= (sample < 0) ? -1 : 1;
            dequantized(band, sampleInd) = dequantizedSample;
        }
    }
}

void FrameSynthesizer::antialias(const GranuleChannelSideInfo& channelInfo) {
    if (channelInfo.windowSwitching)
        throw std::runtime_error("Nope yet");

    for (std::size_t band = 0; band < NR_FREQ_BANDS - 1; band++) {
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

void FrameSynthesizer::polyphaseSynthesis() {
    auto buildMatrix = [&]() {
        auto ret = DoubleMatrix(D_WINDOW_VECTOR_SIZE, NR_D_WINDOW_VECTORS);
        for (std::size_t row = 0; row < NR_D_WINDOW_VECTORS; row++) {
            for (std::size_t col = 0; col < D_WINDOW_VECTOR_SIZE; col++) {
                auto startOffset = (col % 2) ? NR_D_WINDOW_VECTORS : 0;
                ret(row, col) = fifo[col][startOffset + row];
            }
        }

        return ret;
    };

    auto matrixed = timeSamples.transpose() * synthesisFilter;

    for (std::size_t i = 0; i < 18; i++) {
        auto row = matrixed.row(i);
        fifo.pop_back();
        fifo.emplace_front(row.begin(), row.size());
        auto matrix = buildMatrix();
        auto result = SCALE * (dWindow.elemWiseProduct(matrix).collapseRows());
    }
}
