#ifndef AIXA_SINEGENERATOR_H
#define AIXA_SINEGENERATOR_H

#include <cmath>

#include "Vector.h"
#include "types.h"

namespace aixa::math {

    class SineGenerator {
    public:
        SineGenerator(std::size_t signalSize,
                      double samplePeriod,
                      double freq,
                      unsigned int scaleFactor = 1);

        const DoubleVector &nextSignal();

    private:
        static constexpr double MAX_PHASE = 2 * M_PI;

        std::size_t signalSize;
        double samplePeriod;
        double freq;
        unsigned int scaleFactor;

        DoubleVector signal;
        double phase;
    };

}

#endif //AIXA_SINEGENERATOR_H
