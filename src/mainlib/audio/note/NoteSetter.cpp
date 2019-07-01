#include "NoteSetter.h"

#include <cmath>

#include "mainlib/audio/AudioWorker.h"

NoteSetter::NoteSetter(NotesBuffer &notesBuffer, AudioWorker &audioWorker) :
    buffer(&notesBuffer),
    worker(&audioWorker),
    noteSeen(false) { }

void NoteSetter::setNote(float steps) {
    static constexpr double BASE_FREQ = 440;
    static constexpr float SEMITONES = 12;

    double newFreq = pow(2, steps / SEMITONES) * BASE_FREQ;

    worker->setFrequency(newFreq);
}

void NoteSetter::setPitch(unsigned int pitch) {
    assert(pitch < Note::VALID_PITCHES);

    Note::Pitch nPitch = static_cast<Note::Pitch>(pitch);

    Note n;
    n.pitch = nPitch;

    if (!noteSeen) n.octave = DEFAULT_OCTAVE;

    buffer->insert(n);

    noteSeen = true;
}
