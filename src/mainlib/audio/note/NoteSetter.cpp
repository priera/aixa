#include "NoteSetter.h"

#include <cmath>
#include <cassert>

#include "mainlib/audio/note/Note.h"


NoteSetter::NoteSetter(NotesBuffer &notesBuffer) :
    buffer(&notesBuffer),
    noteSeen(false) { }

void NoteSetter::setPitch(Note::Pitch pitch) {
    Note n;
    n.pitch = pitch;

    if (!noteSeen) n.octave = DEFAULT_OCTAVE;

    buffer->insert(n);

    noteSeen = true;
}

void NoteSetter::setOctave(int octave) {
    if (!noteSeen) return;
    if (!(octave >= 0 && octave < 9)) return;

    Note n;
    n.octave = octave;

    buffer->insert(n);
}

void NoteSetter::setModifier(Note::Modifier modifier) {
    if (!noteSeen) return;

    Note n;
    n.modifier = modifier;

    buffer->insert(n);
}