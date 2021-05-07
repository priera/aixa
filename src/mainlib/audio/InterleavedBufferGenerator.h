#ifndef AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERGENERATOR_H
#define AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERGENERATOR_H

#include <mainlib/threading/BuffersRing.h>

#include "InterleavedBuffer.h"

class InterleavedBufferGenerator {
public:
    InterleavedBufferGenerator(unsigned int channels, std::size_t frameSize, unsigned int bytesPerSample) :
        channels(channels), frameSize(frameSize), bytesPerSample(bytesPerSample) {}

    virtual ~InterleavedBufferGenerator() = default;

    BuffersRing<InterleavedBuffer>::ElemGenerator generator();

private:
    unsigned int channels;
    std::size_t frameSize;
    unsigned int bytesPerSample;
};

#endif  // AIXA_SRC_MAINLIB_AUDIO_INTERLEAVEDBUFFERGENERATOR_H
