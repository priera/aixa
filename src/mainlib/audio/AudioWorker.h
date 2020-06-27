#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H

#include <memory>

#include "StreamReader.h"
#include "Publisher.h"

class AudioWorker {
public:
    AudioWorker(AudioEnvironment env,
            std::unique_ptr<StreamReader> reader,
            std::unique_ptr<Publisher> publisher) :
    env(std::move(env)),
    reader(std::move(reader)),
    publisher(std::move(publisher)) {}

    void start();
    void stop();

    virtual ~AudioWorker() = default;

private:
    AudioEnvironment env;
    std::unique_ptr<StreamReader> reader;
    std::unique_ptr<Publisher> publisher;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
