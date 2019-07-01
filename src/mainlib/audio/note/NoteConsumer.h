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
    AudioWorker *worker;
    NotesBuffer *buffer;

    std::atomic<bool> stopVariable;
    
    Note lastNote;
};


#endif //ALSAPLAYGROUND_NOTECONSUMER_H
