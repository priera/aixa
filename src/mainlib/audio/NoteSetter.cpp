#include "NoteSetter.h"

#include <cmath>

#include "mainlib/audio/AudioWorker.h"

void NoteSetter::setNote(float steps) {
    static constexpr double BASE_FREQ = 440;
    static constexpr float SEMITONES = 12;

    double newFreq = pow(2, steps / SEMITONES) * BASE_FREQ;

    worker->setFrequency(newFreq);
}
