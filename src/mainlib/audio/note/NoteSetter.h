#ifndef ALSAPLAYGROUND_NOTESETTER_H
#define ALSAPLAYGROUND_NOTESETTER_H

#include "Note.h"

class AudioWorker;

class NoteSetter {
public:
    NoteSetter(NotesBuffer &notesBuffer, AudioWorker &audioWorker);

    void setNote(float steps);
    void setPitch(unsigned int pitch);

private:
    static constexpr int DEFAULT_OCTAVE = 4;

    AudioWorker *worker;
    NotesBuffer  *buffer;

    bool noteSeen;
};

#endif //ALSAPLAYGROUND_NOTESETTER_H
