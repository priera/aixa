#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H

#include "types.h"

class FrameSynthesizer {
public:
    FrameSynthesizer() : dequantized(NR_FREQ_BANDS) {
        for (auto& band : dequantized) {
            band.resize(NR_SAMPLES_PER_BAND);
        }
    };

    virtual ~FrameSynthesizer() = default;

    void synthesize(unsigned int samplingFreq,
                    const SideInformation& sideInfo,
                    const MainDataContent& content,
                    std::size_t nChannels);

private:
    static constexpr float GAIN_BASE = 210.f;
    static std::vector<unsigned int> pretab;

    void dequantizeSamples(unsigned int samplingFreq,
                           const GranuleChannelSideInfo& channelInfo,
                           const GranuleChannelContent& channelContent);

    FrequencyBands dequantized;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
