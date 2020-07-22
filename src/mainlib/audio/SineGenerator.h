#ifndef AIXA_SINEGENERATOR_H
#define AIXA_SINEGENERATOR_H

#include <cmath>

#include <mainlib/math/Vector.h>
#include <mainlib/math/types.h>

#include "AudioDefinitions.h"

class InterleavedBuffer;

class SineGenerator {
public:
    SineGenerator(std::size_t signalSize, double samplePeriod, double freq, unsigned int scaleFactor = 1);

    const aixa::math::DoubleVector &nextSignal();

private:
    static constexpr double MAX_PHASE = 2 * M_PI;

    std::size_t signalSize;
    double samplePeriod;
    double freq;
    unsigned int scaleFactor;

    aixa::math::DoubleVector signal;
    double phase;
};


#endif //AIXA_SINEGENERATOR_H
