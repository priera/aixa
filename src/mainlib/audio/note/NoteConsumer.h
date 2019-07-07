#ifndef ALSAPLAYGROUND_NOTECONSUMER_H
#define ALSAPLAYGROUND_NOTECONSUMER_H

#include <atomic>

#include "mainlib/audio/note/Note.h"

class AudioWorker;

class NoteConsumer {
public:
    NoteConsumer(AudioWorker &worker, NotesBuffer &buffer);

    void start();
    void stop();

private:
    void mergeNewNote(const Note &incomingNote);

    AudioWorker *worker;
    NotesBuffer *buffer;

    std::atomic<bool> stopVariable;
    
    Note lastNote;
    bool noteSeen;
    time_t previousPts;
};


#endif //ALSAPLAYGROUND_NOTECONSUMER_H
