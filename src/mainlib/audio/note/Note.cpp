#include "mainlib/audio/note/Note.h"

#include <cmath>

static constexpr double BASE_FREQ = 440;
static constexpr float SEMITONES = 12;
static constexpr float CONCERT_A_SEMIS = static_cast<unsigned char>(Note::Pitch::A) + SEMITONES * 4;

double computeFrequency(const Note &n) {
    float noteSemis = static_cast<unsigned char>(n.pitch) + (n.octave * SEMITONES);
    float steps = noteSemis - CONCERT_A_SEMIS;

    if (n.modifier != Note::Modifier::NONE)
        steps += (n.modifier == Note::Modifier::SHARP) ? 1 : -1;

    double newFreq = std::pow(2, steps / SEMITONES) * BASE_FREQ;

    return newFreq;
}

char getNoteChar(const Note &n) {
    char ret = ' ';
    switch (n.pitch) {
        case Note::Pitch::A:
            ret = 'A';
            break;
        case Note::Pitch::B:
            ret = 'B';
            break;
        case Note::Pitch::C:
            ret = 'C';
            break;
        case Note::Pitch::D:
            ret = 'D';
            break;
        case Note::Pitch::E:
            ret = 'E';
            break;
        case Note::Pitch::F:
            ret = 'F';
            break;
        case Note::Pitch::G:
            ret = 'G';
            break;
    }

    return ret;
}