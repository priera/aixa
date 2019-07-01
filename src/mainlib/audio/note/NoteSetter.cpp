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
