#include "SineGenerator.h"

#include <utility>

#include "InterleavedBuffer.h"

SineGenerator::SineGenerator(std::shared_ptr<SamplesRing> samplesRing, int frameSize, double rate) :
    samplesRing(std::move(samplesRing)),
    frameSize(frameSize),
    rate(rate),
    phase(0) {}

void SineGenerator::fillFrame(double freq, unsigned int scaleFactor) {
    double step = MAX_PHASE * freq / rate;

    auto buffer = samplesRing->nextWriteBuffer();

    buffer->startNewFrame();

    int n = frameSize;
    while (n-- > 0) {
        int res;

        res = sin(phase) * scaleFactor;
        buffer->storeNextSample(res);

        phase += step;
        if (phase >= MAX_PHASE) {
            phase -= MAX_PHASE;
        }
    }
}