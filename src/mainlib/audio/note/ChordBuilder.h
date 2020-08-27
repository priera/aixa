#ifndef AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORDBUILDER_H
#define AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORDBUILDER_H

#include <utility>

#include "Note.h"
#include "Chord.h"

class ChordBuilder {
public:
    static ChordBuilder triad(Note tonic);
    static ChordBuilder seventh(Note tonic);

    Chord minor();
    Chord augmented();
    Chord diminished();

    Chord build() const;

private:
    ChordBuilder(Note tonic, std::vector<int> semitones) :
        tonic(tonic),
        semitones(std::move(semitones)),
        buildingSeventh(semitones.size() == 4) {
    }

    Note tonic;
    std::vector<int> semitones;

    bool buildingSeventh;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORDBUILDER_H
