#ifndef AIXA_SINEGENERATOR_H
#define AIXA_SINEGENERATOR_H

#include <cmath>

#include "AudioDefinitions.h"

class InterleavedBuffer;

class SineGenerator {
public:
    SineGenerator(std::shared_ptr<SamplesRing> samplesRing, int frameSize, double rate);

    void fillFrame(double freq, unsigned int scaleFactor);

private:
    static constexpr double MAX_PHASE = 2 * M_PI;

    std::shared_ptr<SamplesRing> samplesRing;
    int frameSize;
    double rate;

    double phase;
};


#endif //AIXA_SINEGENERATOR_H
