#include "VolumeManager.h"

void VolumeManager::applyTo(InterleavedBuffer &buff) {
    auto lVolume = volume;
    auto size = buff.dataSize();

    auto samplesBytes = &buff.frame()[0];
    auto inc = sizeof(short);
    for (size_t i = 0; i < size; i += inc) {
        auto sample = reinterpret_cast<short *>(samplesBytes);
        *sample *= lVolume;
        samplesBytes += inc;
    }
}

void VolumeManager::increaseVolume() {
    addToVolume(VOLUME_STEP);
}

void VolumeManager::decreaseVolume() {
    addToVolume(-VOLUME_STEP);
}

void VolumeManager::addToVolume(float amount) {
    auto lVolume = volume;
    lVolume += amount;

    if (lVolume < MIN_VOLUME)
        lVolume = MIN_VOLUME;
    else if (lVolume > MAX_VOLUME)
        lVolume = MAX_VOLUME;

    volume = lVolume;
}
