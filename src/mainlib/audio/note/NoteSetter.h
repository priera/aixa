#ifndef ALSAPLAYGROUND_NOTESETTER_H
#define ALSAPLAYGROUND_NOTESETTER_H

#include "Note.h"

class NoteSetter {
public:
    NoteSetter(NotesBuffer &notesBuffer);

    void setPitch(Note::Pitch pitch);

private:
    static constexpr int DEFAULT_OCTAVE = 4;

    NotesBuffer  *buffer;

    bool noteSeen;
};

#endif //ALSAPLAYGROUND_NOTESETTER_H
