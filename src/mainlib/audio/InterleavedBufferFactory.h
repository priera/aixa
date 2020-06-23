#ifndef AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERFACTORY_H
#define AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERFACTORY_H

#include <mainlib/buffers/BuffersRing.h>

#include "InterleavedBuffer.h"

class InterleavedBufferFactory {
public:
    InterleavedBufferFactory(int channels, snd_pcm_sframes_t frameSize, snd_pcm_format_t format) :
            channels(channels), frameSize(frameSize), format(format) { }

    virtual ~InterleavedBufferFactory() = default;

    BuffersRing<InterleavedBuffer>::ElemGenerator generator();

private:
    int channels;
    snd_pcm_sframes_t frameSize;
    snd_pcm_format_t format;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERFACTORY_H
