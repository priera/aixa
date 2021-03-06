#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H

#include <mainlib/math/types.h>

#include "types.h"

class FrameSynthesizer {
public:
    FrameSynthesizer();
    virtual ~FrameSynthesizer() = default;

    void synthesize(unsigned int samplingFreq,
                    const SideInformation& sideInfo,
                    const MainDataContent& content,
                    std::size_t nChannels);

private:
    static constexpr float GAIN_BASE = 210.f;
    static constexpr std::size_t NR_BUTTERFLIES = 8;

    static std::vector<unsigned int> pretab;

    void initAntialiasCoefficients();
    void initTransformMatrix();
    void initBlockWindows();

    void dequantizeSamples(unsigned int samplingFreq,
                           const GranuleChannelSideInfo& channelInfo,
                           const GranuleChannelContent& channelContent);
    void antialias(const GranuleChannelSideInfo& channelInfo);
    void inverseMDCT(const GranuleChannelSideInfo& info, FrequencyBands<double>& overlappingTerms);

    struct {
        std::array<double, NR_BUTTERFLIES> ca;
        std::array<double, NR_BUTTERFLIES> cs;
    } antialiasCoefficients;
    FrequencyBands<double> dequantized;
    aixa::math::DoubleMatrix cosineTransformMatrix;
    std::map<GranuleChannelSideInfo::BlockType, aixa::math::DoubleMatrix> blockWindows;
    std::array<FrequencyBands<double>, NR_CHANNELS> channelOverlappingTerms;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
