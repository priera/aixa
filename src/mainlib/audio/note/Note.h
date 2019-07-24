#ifndef ALSAPLAYGROUND_NOTE_H
#define ALSAPLAYGROUND_NOTE_H

#include "mainlib/buffers/CircularBuffer.h"

#include "mainlib/observer/Observer.h"

struct Note {
    static constexpr int VALID_PITCHES = 8;

    enum class Pitch : unsigned char {
        C = 0,
        D = 2,
        E = 3,
        F = 5,
        G = 7,
        A = 9,
        B = 11,
        NONE
    };

    enum class Modifier {
        NONE,
        SHARP,
        FLAT
    };

    Note() : pitch(Pitch::NONE), octave(0), modifier(Modifier::NONE) {}

    Pitch pitch;
    int octave;
    Modifier modifier;
};

double computeFrequency(const Note &n);
char getNoteChar(const Note &n);

using NotesBuffer = CircularBuffer<Note>;
using NotesListener = Observer<Note>;

#endif //ALSAPLAYGROUND_NOTE_H
