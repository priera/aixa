#include "LongWindowScaleFactorsComputer.h"

#include <cmath>

std::vector<unsigned int> LongWindowScaleFactorsComputer::pretab = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                    1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0};

Bands<double> LongWindowScaleFactorsComputer::compute(unsigned int samplingFreq,
                                                      const GranuleChannelSideInfo& channelInfo,
                                                      const GranuleChannelContent& channelContent) {
    auto ret = Bands<double>();
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
            ret[band][sampleInd] = std::pow(2.0, exp);
        }
    }

    return ret;
}
