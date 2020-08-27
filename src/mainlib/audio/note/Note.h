#ifndef AIXA_NOTE_H
#define AIXA_NOTE_H

#include "mainlib/threading/CircularBuffer.h"

#include "mainlib/observer/Observer.h"

struct Note {
    enum class Pitch : signed int {
        A = 0,
        B = 2,
        C = 3,
        D = 5,
        E = 7,
        F = 8,
        G = 10
    };

    enum class Modifier : signed int {
        NONE    = 0,
        SHARP   = +1,
        FLAT    = -1
    };

    Note(Pitch pitch, int octave, Modifier modifier)
            : pitch(pitch),
              octave(octave),
              modifier(modifier) { }

    Note() : Note(Pitch::A, 4, Modifier::NONE) {}

    Pitch pitch;
    int octave;
    Modifier modifier;
};

double computeFrequency(unsigned int octave, int semitones);
char getNoteChar(const Note &n);

using NotesBuffer = CircularBuffer<Note>;
using NotesListener = Observer<Note>;

#endif //AIXA_NOTE_H
