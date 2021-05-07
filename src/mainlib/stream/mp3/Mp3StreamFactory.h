#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAMFACTORY_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAMFACTORY_H

#include <mainlib/stream/StreamFactory.h>

#include "Mp3Decoder.h"

class Mp3StreamFactory : public StreamFactory {
public:
    explicit Mp3StreamFactory(std::string streamPath) : streamPath(std::move(streamPath)) {}

    std::shared_ptr<Stream> probe() override;

private:
    static constexpr std::size_t PROBING_FRAMES = 4;

    Mp3Decoder* buildDecoder();
    void probeWithDecoder(Mp3Decoder& decoder, FrameHeader& header);

    std::string streamPath;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAMFACTORY_H
