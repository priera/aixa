#ifndef AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERGENERATOR_H
#define AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERGENERATOR_H

#include <mainlib/threading/BuffersRing.h>

#include "InterleavedBuffer.h"

class InterleavedBufferGenerator {
public:
    InterleavedBufferGenerator(unsigned int channels, std::size_t frameSize, snd_pcm_format_t format) :
        channels(channels), frameSize(frameSize), format(format) {}

    virtual ~InterleavedBufferGenerator() = default;

    BuffersRing<InterleavedBuffer>::ElemGenerator generator();

private:
    unsigned int channels;
    std::size_t frameSize;
    snd_pcm_format_t format;
};

#endif  // AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERGENERATOR_H
