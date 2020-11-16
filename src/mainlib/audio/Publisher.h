#ifndef AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H
#define AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H

#include <mainlib/math/dft/SpectrogramComputer.h>

#include "AudioDefinitions.h"
#include "VolumeManager.h"

using namespace aixa::math;

class Publisher {
public:
    explicit Publisher(AlsaEnvironment alsaEnv,
        std::shared_ptr<SamplesRing> samplesRing,
        std::unique_ptr<VolumeManager> volumeManager,
        std::unique_ptr<SpectrogramComputer> spectrogramComputer) :
            alsaEnv(std::move(alsaEnv)),
            samplesRing(std::move(samplesRing)),
            volumeManager(std::move(volumeManager)),
            spectrogramComputer(std::move(spectrogramComputer)),
            sleepTime(std::chrono::microseconds(this->alsaEnv.params.period_time)) {}

    virtual ~Publisher() = default;

    bool exec();

    void increaseVolume() { volumeManager->increaseVolume(); }

    void decreaseVolume() { volumeManager->decreaseVolume(); }

    SpectrogramGenerator &getSpectrogramGenerator() {
        return *spectrogramComputer;
    }

private:
    void attemptStreamRecovery(int err);

    void waitForStream();

    AlsaEnvironment alsaEnv;
    std::shared_ptr<SamplesRing> samplesRing;
    std::unique_ptr<VolumeManager> volumeManager;
    std::unique_ptr<aixa::math::SpectrogramComputer> spectrogramComputer;
    std::chrono::microseconds sleepTime;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_PUBLISHER_H
