#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H

#include <memory>

#include <mainlib/threading/WorkerThread.h>
#include <mainlib/CommandListener.h>

#include "StreamReader.h"
#include "Publisher.h"

using namespace aixa::math;

class AudioWorker : public CommandListener {
public:
    AudioWorker(AudioEnvironment env,
            std::unique_ptr<StreamReader> reader,
            std::unique_ptr<Publisher> publisher);

    ~AudioWorker() override = default;

    CommandCollection getCommandCollection() override;

    SpectrogramGenerator &getSpectrogramGenerator() {
        return publisher->getSpectrogramGenerator();
    }

    void start();
    void stop();

    void increaseVolume();
    void decreaseVolume();

private:
    using ReadingThread = WorkerThread<StreamReader>;
    using PublishingThread = WorkerThread<Publisher>;

    AudioEnvironment env;
    std::unique_ptr<StreamReader> reader;
    std::unique_ptr<Publisher> publisher;

    ReadingThread readingThread;
    PublishingThread publishingThread;

    CommandCollection myCommands;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
