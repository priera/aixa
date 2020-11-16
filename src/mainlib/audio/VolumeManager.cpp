#include "VolumeManager.h"

void VolumeManager::applyTo(InterleavedBuffer &buff) {
    float lVolume = volume;

    auto samples = buff.samples();
    for (size_t i = 0; i < buff.samplesCount(); i++) {
        samples[i] *= lVolume;
    }
}

void VolumeManager::increaseVolume() {
    addToVolume(VOLUME_STEP);
}

void VolumeManager::decreaseVolume() {
    addToVolume(-VOLUME_STEP);
}

void VolumeManager::addToVolume(float amount) {
    float lVolume = volume;
    lVolume += amount;

    if (lVolume < MIN_VOLUME)
        lVolume = MIN_VOLUME;
    else if (lVolume > MAX_VOLUME)
        lVolume = MAX_VOLUME;

    volume = lVolume;
}
