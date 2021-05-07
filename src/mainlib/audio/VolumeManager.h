#ifndef AIXA_SRC_MAINLIB_AUDIO_VOLUMEMANAGER_H
#define AIXA_SRC_MAINLIB_AUDIO_VOLUMEMANAGER_H

#include "InterleavedBuffer.h"

class VolumeManager {
public:

    virtual ~VolumeManager() = default;

    void applyTo(InterleavedBuffer &buff);
    void increaseVolume();
    void decreaseVolume();

private:
    void addToVolume(float amount);

    static constexpr float MIN_VOLUME = 0;
    static constexpr float MAX_VOLUME = 1.0;
    static constexpr float VOLUME_STEP = (MAX_VOLUME - MIN_VOLUME) / 20;

    volatile float volume{0.6};

};


#endif //AIXA_SRC_MAINLIB_AUDIO_VOLUMEMANAGER_H
