#ifndef ALSAPLAYGROUND_NOTE_H
#define ALSAPLAYGROUND_NOTE_H

#include "mainlib/buffers/CircularBuffer.h"

struct Note {
    static constexpr int VALID_PITCHES = 8;

    enum class Pitch {
        A = 0,
        B,
        C,
        D,
        E,
        F,
        G,
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



using NotesBuffer = CircularBuffer<Note>;

#endif //ALSAPLAYGROUND_NOTE_H
