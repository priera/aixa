#ifndef AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H
#define AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H

#include "AudioDefinitions.h"
#include "VolumeManager.h"

class Publisher {
public:
    Publisher(AlsaEnvironment alsaEnv,
            std::shared_ptr<SamplesRing> samplesRing,
            std::unique_ptr<VolumeManager> volumeManager) :
            alsaEnv(std::move(alsaEnv)),
            samplesRing(std::move(samplesRing)),
            volumeManager(std::move(volumeManager)),
            sleepTime(std::chrono::microseconds(this->alsaEnv.params.period_time)) {}

    virtual ~Publisher() = default;

    bool exec();

    void increaseVolume() { volumeManager->increaseVolume(); }

    void decreaseVolume() { volumeManager->decreaseVolume(); }

private:
    void attemptStreamRecovery(int err);

    void waitForStream();

    AlsaEnvironment alsaEnv;
    std::shared_ptr<SamplesRing> samplesRing;
    std::unique_ptr<VolumeManager> volumeManager;
    std::chrono::microseconds sleepTime;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H
