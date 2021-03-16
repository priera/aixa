#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H

#include <mainlib/math/types.h>

#include "types.h"

class FrameSynthesizer {
public:
    static constexpr std::size_t NR_BUTTERFLIES = 8;

    struct AntialiasCoefficients {
        std::array<double, NR_BUTTERFLIES> ca;
        std::array<double, NR_BUTTERFLIES> cs;
    };

    FrameSynthesizer(AntialiasCoefficients antialiasCoefficients, aixa::math::DoubleMatrix cosineTransform);
    virtual ~FrameSynthesizer() = default;

    void synthesize(unsigned int samplingFreq,
                    const SideInformation& sideInfo,
                    const MainDataContent& content,
                    std::size_t nChannels);

private:
    static constexpr float GAIN_BASE = 210.f;

    static std::vector<unsigned int> pretab;

    void initBlockWindows();
    void initFrequencyInversionMatrix();
    void initTimeDomainSynFilter();

    void dequantizeSamples(unsigned int samplingFreq,
                           const GranuleChannelSideInfo& channelInfo,
                           const GranuleChannelContent& channelContent);
    void antialias(const GranuleChannelSideInfo& channelInfo);
    void inverseMDCT(const GranuleChannelSideInfo& info, Bands<double>& overlappingTerms);
    void polyphaseSynthesis();

    AntialiasCoefficients antialiasCoefficients;
    aixa::math::DoubleMatrix cosineTransform;
    aixa::math::DoubleMatrix timeSamples;
    aixa::math::DoubleMatrix frequencyInversion;
    aixa::math::DoubleMatrix synthesisFilter;

    aixa::math::DoubleMatrix dequantized;
    std::map<GranuleChannelSideInfo::BlockType, aixa::math::DoubleMatrix> blockWindows;
    std::array<Bands<double>, NR_CHANNELS> channelOverlappingTerms;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
