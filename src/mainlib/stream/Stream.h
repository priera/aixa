#ifndef AIXA_SRC_MAINLIB_STREAM_STREAM_H
#define AIXA_SRC_MAINLIB_STREAM_STREAM_H

#include <mainlib/audio/AudioDefinitions.h>

class Stream {
public:
    virtual ~Stream() = default;

    virtual AudioStreamParameters getParameters() const = 0;
    virtual short nextSample() = 0;
    virtual bool finished() = 0;
};

#endif //AIXA_SRC_MAINLIB_STREAM_STREAM_H
