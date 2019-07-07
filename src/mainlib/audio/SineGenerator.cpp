#include "SineGenerator.h"

#include "Buffers.h"

SineGenerator::SineGenerator(Buffers &buffers, int frameSize, double rate) :
    buffers(&buffers),
    frameSize(frameSize),
    rate(rate),
    phase(0) {}

void SineGenerator::fillFrame(double freq, unsigned int scaleFactor) {
    double step = MAX_PHASE * freq / rate;

    buffers->startNewFrame();

    int n = frameSize;
    while (n-- > 0) {
        int res;

        res = sin(phase) * scaleFactor;
        buffers->storeNextSample(res);

        phase += step;
        if (phase >= MAX_PHASE) {
            phase -= MAX_PHASE;
        }
    }
}