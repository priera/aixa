#ifndef AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H
#define AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H

#include "AudioDefinitions.h"

class Publisher {
public:
    Publisher(AlsaEnvironment alsaEnv, std::shared_ptr<SamplesRing> samplesRing) :
    alsaEnv(std::move(alsaEnv)),
    samplesRing(std::move(samplesRing)),
    sleepTime(std::chrono::microseconds(this->alsaEnv.params.period_time)){}

    virtual ~Publisher() = default;

    void exec();

private:
    void attemptStreamRecovery(int err);
    void waitForStream();

    AlsaEnvironment alsaEnv;
    std::shared_ptr<SamplesRing> samplesRing;
    std::chrono::microseconds sleepTime;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H
