#include "mainlib/audio/note/Note.h"

#include <cmath>

static constexpr double BASE_FREQ = 440;
static constexpr float SEMITONES = 12;
static constexpr float CONCERT_A_SEMIS = static_cast<unsigned char>(Note::Pitch::A) + SEMITONES * 4;

double computeFrequency(const Note &n) {
    float noteSemis = static_cast<unsigned char>(n.pitch) + (n.octave * SEMITONES);
    float steps = noteSemis - CONCERT_A_SEMIS;

    double newFreq = std::pow(2, steps / SEMITONES) * BASE_FREQ;

    return newFreq;
}