#include "SineGenerator.h"

namespace aixa::math {
    SineGenerator::SineGenerator(std::size_t signalSize,
                                 double samplePeriod,
                                 double freq,
                                 unsigned int scaleFactor) :
            signalSize(signalSize),
            samplePeriod(samplePeriod),
            freq(freq),
            scaleFactor(scaleFactor),
            signal(DoubleVector(signalSize)),
            phase(0) {}

    const DoubleVector &SineGenerator::nextSignal() {
        auto lFreq = freq;
        auto lScaleFactor = scaleFactor;

        double step = MAX_PHASE * lFreq * samplePeriod;

        for (size_t i = 0; i < signalSize; i++) {
            signal[i] = std::sin(phase) * lScaleFactor;

            phase += step;
            if (phase >= MAX_PHASE) {
                phase -= MAX_PHASE;
            }
        }

        return signal;
    }
}
