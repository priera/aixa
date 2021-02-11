#include "FrameSynthesizer.h"

#include <cmath>
#include <stdexcept>

std::vector<unsigned int> FrameSynthesizer::pretab = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0};

void FrameSynthesizer::initAntialiasCoefficients() {
    const std::vector<double> ci = {-0.6, -0.535, -0.33, -0.185, -0.095, -0.041, -0.0142, -0.0037};

    for (int i = 0; i < NR_BUTTERFLIES; i++) {
        auto sq = std::sqrt(1.0 + ci[i] * ci[i]);
        antialiasCoefficients.cs[i] = 1.0 / sq;
        antialiasCoefficients.ca[i] = ci[i] / sq;
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
        for (std::size_t sampleInd = 0; sampleInd < NR_SAMPLES_PER_BAND; sampleInd++) {
            if ((band * NR_SAMPLES_PER_BAND) + sampleInd == nextSubbandBoundary) {
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
            dequantized[band][sampleInd] = dequantizedSample;
        }
    }
}

void FrameSynthesizer::antialias(const GranuleChannelSideInfo& channelInfo) {
    if (channelInfo.windowSwitching)
        throw std::runtime_error("Nope yet");

    for (std::size_t band = 0; band < NR_FREQ_BANDS - 1; band++) {
        for (std::size_t butterfly = 0; butterfly < NR_BUTTERFLIES; butterfly++) {
            auto upIndex = NR_SAMPLES_PER_BAND - 1 - butterfly;
            auto bandUp = dequantized[band][upIndex];
            auto bandDown = dequantized[band + 1][butterfly];
            dequantized[band][upIndex] =
                antialiasCoefficients.cs[butterfly] * bandUp - antialiasCoefficients.ca[butterfly] * bandDown;
            dequantized[band + 1][butterfly] =
                antialiasCoefficients.ca[butterfly] * bandUp + antialiasCoefficients.cs[butterfly] * bandDown;
        }
    }
}
