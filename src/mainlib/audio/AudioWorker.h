#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H

#include <memory>

#include <mainlib/threading/WorkerThread.h>

#include "StreamReader.h"
#include "Publisher.h"

class AudioWorker {
public:
    AudioWorker(AudioEnvironment env,
            std::unique_ptr<StreamReader> reader,
            std::unique_ptr<Publisher> publisher) :
    env(std::move(env)),
    reader(std::move(reader)),
    publisher(std::move(publisher)),
    readingThread(*this->reader),
    publishingThread(*this->publisher) {}

    virtual ~AudioWorker() = default;

    void start();
    void stop();

private:
    using ReadingThread = WorkerThread<StreamReader>;
    using PublishingThread = WorkerThread<Publisher>;

    AudioEnvironment env;
    std::unique_ptr<StreamReader> reader;
    std::unique_ptr<Publisher> publisher;

    ReadingThread readingThread;
    PublishingThread publishingThread;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
