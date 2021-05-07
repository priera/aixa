#include "mainlib/audio/note/Note.h"

#include <cmath>

static constexpr double BASE_FREQ = 440;
static constexpr int OCTAVE_SEMITONES = 12;
static constexpr int CONCERT_A_STEPS = 4 * OCTAVE_SEMITONES;

double computeFrequency(unsigned int octave, int semitones) {
    // https://en.wikipedia.org/wiki/Musical_note#Note_frequency_(hertz)
    auto steps = static_cast<int>(octave * OCTAVE_SEMITONES + semitones) - CONCERT_A_STEPS;
    auto exp = static_cast<double>(steps) / OCTAVE_SEMITONES;
    return std::pow(2, exp)  * BASE_FREQ;
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