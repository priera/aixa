#include "ChordBuilder.h"

#include <algorithm>

ChordBuilder ChordBuilder::triad(Note tonic) {
    std::vector<int> semitones = {0, 4, 7};
    return std::move(ChordBuilder(tonic, semitones));
}

ChordBuilder ChordBuilder::seventh(Note tonic) {
    std::vector<int> semitones = {0, 4, 7, 11};
    return ChordBuilder(tonic, semitones);
}

Chord ChordBuilder::minor() {
    semitones[1]--;

    if (buildingSeventh)
        semitones[3]--;

    return build();
}

Chord ChordBuilder::augmented() {
    semitones[2]++;

    if (buildingSeventh)
        semitones[3]--;

    return build();
}

Chord ChordBuilder::diminished() {
    semitones[1]--;
    semitones[2]--;

    return build();
}

Chord ChordBuilder::build() const {
    std::vector<double> notes;
    int tonicSemis = static_cast<signed int>(tonic.pitch) + static_cast<signed int>(tonic.modifier);
    int tonicOctave = tonic.octave;
    std::transform(semitones.begin(), semitones.end(), std::back_inserter(notes),
                   [tonicOctave, tonicSemis](int semis) -> double {
        int noteSemis = tonicSemis + semis;
        return computeFrequency(tonicOctave, noteSemis);
    });

    return Chord(notes);
}
