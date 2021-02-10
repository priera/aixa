#include "FrameSynthesizer.h"

#include <cmath>
#include <stdexcept>

std::vector<unsigned int> FrameSynthesizer::pretab = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0};

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
            // antialias();
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
