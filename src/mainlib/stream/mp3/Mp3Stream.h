#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAM_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAM_H

#include <mainlib/stream/Stream.h>

#include "Mp3Decoder.h"

class Mp3Stream : public Stream {
public:
    Mp3Stream(FrameHeader streamDefinition, std::unique_ptr<Mp3Decoder> decoder) :
        streamDefinition(streamDefinition), decoder(std::move(decoder)), samples(), alreadyEnded(false),
        previousIterCopied(0) {}

    AudioStreamParameters getParameters() const override;
    void prepareForFirstRead() override;
    bool ended() override;
    void storeSamples(InterleavedBuffer &buffer) override;

private:
    const FrameHeader streamDefinition;
    std::unique_ptr<Mp3Decoder> decoder;
    FrameSamples samples;

    bool alreadyEnded;
    std::size_t previousIterCopied;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAM_H
