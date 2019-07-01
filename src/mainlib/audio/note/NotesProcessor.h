#ifndef ALSAPLAYGROUND_NOTESPROCESSOR_H
#define ALSAPLAYGROUND_NOTESPROCESSOR_H

#include <memory>
#include <thread>

#include "mainlib/audio/note/Note.h"

class NoteSetter;
class AudioWorker;
class NoteConsumer;

class NotesProcessor {
public:
    NotesProcessor(AudioWorker &audioWorker);
    virtual ~NotesProcessor();


    NoteSetter *getNoteSetter() { return noteSetter.get(); }
    void start();
    void stop();

private:
    AudioWorker *audioWorker;

    bool running;
    std::unique_ptr<NoteConsumer> consumer;
    std::unique_ptr<std::thread> consumerThread;

    std::unique_ptr<NotesBuffer> buffer;
    std::unique_ptr<NoteSetter> noteSetter;
};


#endif //ALSAPLAYGROUND_NOTESPROCESSOR_H
