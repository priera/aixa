#ifndef ALSAPLAYGROUND_AUDIOWORKER_H
#define ALSAPLAYGROUND_AUDIOWORKER_H

#include <atomic>
#include <string>
#include <memory>

#include <alsa/asoundlib.h>
#include "AudioDefinitions.h"

class AudioWorker {
public:
    AudioWorker(const AudioParameters &params, std::unique_ptr<AlsaEnvironment> &environment);

    void start();
    void stop();

private:
    void writeLoop();

    AudioParameters params;
    std::unique_ptr<AlsaEnvironment> environment;

    std::atomic<bool> stopValue;
};


#endif //ALSAPLAYGROUND_AUDIOWORKER_H
