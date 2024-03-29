#ifndef AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORDBUILDER_H
#define AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORDBUILDER_H

#include <utility>

#include <platform/aixa_export.h>

#include "Note.h"
#include "Chord.h"

class LIB_EXPORT ChordBuilder {
public:
    static ChordBuilder triad(Note tonic);
    static ChordBuilder seventh(Note tonic);

    Chord minor();
    Chord augmented();
    Chord diminished();

    operator Chord() const {
        return this->build();
    }

private:
    ChordBuilder(Note tonic, std::vector<int> semitones) :
        tonic(tonic),
        semitones(std::move(semitones)),
        buildingSeventh(semitones.size() == 4) {
    }

    Chord build() const;

    Note tonic;
    std::vector<int> semitones;

    bool buildingSeventh;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_NOTE_CHORDBUILDER_H
