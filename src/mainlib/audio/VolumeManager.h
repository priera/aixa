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
    static constexpr unsigned int MIN_VOLUME = 0;

    static constexpr int FORMAT_BITS = 16; //snd_pcm_format_width(SND_PCM_FORMAT_S16);
    static constexpr unsigned int MAX_VOLUME = (1 << (FORMAT_BITS - 1)) - 1;
    static constexpr unsigned int VOLUME_STEP = (MAX_VOLUME - MIN_VOLUME) / 20;

    volatile unsigned int volume{0};

};


#endif //AIXA_SRC_MAINLIB_AUDIO_VOLUMEMANAGER_H
