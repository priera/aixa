#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H

#include <aixa_export.h>
#include <mainlib/CommandListener.h>
#include <mainlib/threading/WorkerThread.h>

#include <memory>

#include "AudioProcessingThread.h"
#include "StreamReader.h"

using namespace aixa::math;

class LIB_EXPORT AudioWorker : public CommandListener {
public:
    AudioWorker(std::unique_ptr<StreamReader> reader,
                std::unique_ptr<AudioProcessingThread> processingThread);

    ~AudioWorker() override = default;

    CommandCollection getCommandCollection() override;

    SpectrogramGenerator &getSpectrogramGenerator() { return processingThread->getSpectrogramGenerator(); }

    void start();
    void stop();

    void increaseVolume();
    void decreaseVolume();

private:
    using ReadingThread = WorkerThread<StreamReader>;

    std::unique_ptr<StreamReader> reader;

    ReadingThread readingThread;
    std::unique_ptr<AudioProcessingThread> processingThread;

    CommandCollection myCommands;
    std::condition_variable cvProcessingThread;

    bool processingThreadStopped;
};

#endif  // AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
