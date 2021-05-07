#ifndef AIXA_SRC_MAINLIB_STREAM_STREAM_H
#define AIXA_SRC_MAINLIB_STREAM_STREAM_H

#include <mainlib/audio/AudioDefinitions.h>

class Stream {
public:
    virtual ~Stream() = default;

    virtual QAudioFormat getParameters() const = 0;
    virtual void prepareForFirstRead() = 0;
    virtual bool ended() = 0;
    virtual void storeSamples(InterleavedBuffer &buffer) = 0;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_STREAM_H
