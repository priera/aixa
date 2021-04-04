#include "ShortWindowScaleFactorsComputer.h"

#include <cmath>

Bands<double> ShortWindowScaleFactorsComputer::compute(unsigned int samplingFreq,
                                                       const GranuleChannelSideInfo& channelInfo,
                                                       const GranuleChannelContent& channelContent) {
    auto ret = Bands<double>();

    std::size_t sfBandInd = 0;
    std::size_t nextSubbandBoundary;
    std::size_t channelBandWidth;
    std::size_t channelBandBegin;

    const auto& bandIndexes = samplingFreqBandIndexes[samplingFreq];

    if (channelInfo.mixedBlockFlag) {
        nextSubbandBoundary = bandIndexes.longWindow[1];
    } else {
        auto nextSubbandBegin = bandIndexes.shortWindow[1];
        nextSubbandBoundary = nextSubbandBegin * 3;
        channelBandWidth = nextSubbandBegin;
        channelBandBegin = 0;
    }

    for (std::size_t band = 0; band < NR_FREQ_BANDS; band++) {
        for (std::size_t sampleInd = 0; sampleInd < NR_CODED_SAMPLES_PER_BAND; sampleInd++) {
            auto sample = (band * NR_CODED_SAMPLES_PER_BAND) + sampleInd;
            if (sample == nextSubbandBoundary) {
                if (channelInfo.mixedBlockFlag) {
                    if (sample < bandIndexes.longWindow[8]) {
                        sfBandInd++;
                        nextSubbandBoundary = bandIndexes.longWindow[sfBandInd + 1];
                    } else {
                        if (sample == bandIndexes.longWindow[8]) {
                            sfBandInd = 3;
                        } else {
                            sfBandInd++;
                        }
                        nextSubbandBoundary = bandIndexes.shortWindow[sfBandInd + 1] * NR_SHORT_WINDOWS;
                        channelBandWidth =
                            bandIndexes.shortWindow[sfBandInd + 1] - bandIndexes.shortWindow[sfBandInd];
                        channelBandBegin = bandIndexes.shortWindow[sfBandInd] * NR_SHORT_WINDOWS;
                    }
                } else {
                    sfBandInd++;
                    nextSubbandBoundary = bandIndexes.shortWindow[sfBandInd + 1] * NR_SHORT_WINDOWS;
                    channelBandWidth =
                        bandIndexes.shortWindow[sfBandInd + 1] - bandIndexes.shortWindow[sfBandInd];
                    channelBandBegin = bandIndexes.shortWindow[sfBandInd] * NR_SHORT_WINDOWS;
                }
            }

            double sfTerm;
            if (channelInfo.mixedBlockFlag && band < 2) {
                // same logic as long windows
                auto scaleFactor = channelContent.longWindowScaleFactorBands[sfBandInd - 1];
                double preTabFactor = channelInfo.preFlag * pretab[sfBandInd - 1];
                double exp = -0.5 * (1.0 + channelInfo.scaleFactorScale) * (scaleFactor + preTabFactor);
                sfTerm = std::pow(2.0, exp);
            } else {
                auto window = (sample - channelBandBegin) / channelBandWidth;
                double gain = std::pow(2.0, 0.25 * -8.0 * channelInfo.subBlockGain[window]);
                int sf = channelContent.shortWindowScaleFactorBands[window][sfBandInd];
                double scaledSf = std::pow(2.0, 0.25 * -2.0 * sf * (1.0 + channelInfo.scaleFactorScale));
                sfTerm = gain * scaledSf;
            }
            ret[band][sampleInd] = sfTerm;
        }
    }

    return ret;
}
