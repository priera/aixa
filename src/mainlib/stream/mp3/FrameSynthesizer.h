#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H

#include "types.h"

class FrameSynthesizer {
public:
    virtual ~FrameSynthesizer() = default;

    void synthesize(const SideInformation& sideInfo, const MainDataContent& content, std::size_t nChannels);

private:
    void requantizeSamples(const GranuleChannelContent& channelContent);
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
