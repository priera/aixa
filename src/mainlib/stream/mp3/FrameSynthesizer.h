#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H

#include "types.h"

class FrameSynthesizer {
public:
    FrameSynthesizer() : antialiasCoefficients(), dequantized() { initAntialiasCoefficients(); }
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

    void dequantizeSamples(unsigned int samplingFreq,
                           const GranuleChannelSideInfo& channelInfo,
                           const GranuleChannelContent& channelContent);
    void antialias(const GranuleChannelSideInfo& channelInfo);

    struct {
        std::array<double, NR_BUTTERFLIES> ca;
        std::array<double, NR_BUTTERFLIES> cs;
    } antialiasCoefficients;
    FrequencyBands<double> dequantized;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
