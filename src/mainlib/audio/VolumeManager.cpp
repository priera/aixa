#include "VolumeManager.h"

#include <iostream>

void VolumeManager::applyTo(InterleavedBuffer &buff) {
    auto lVolume = volume;
    auto size = buff.dataSize();

    //FIXME: not working and adds noise
    for (size_t i = 0; i < size; i += 2) {
        unsigned short sample = buff[i];
        sample += (buff[i + 1] << 8);
        sample *= lVolume;
        buff[i] = sample;
    }
}

void VolumeManager::increaseVolume() {
    auto lVolume = volume;
    lVolume += VOLUME_STEP;
    if (lVolume > MAX_VOLUME) lVolume = MAX_VOLUME;
    volume = lVolume;
    std::cout << "increase: " << lVolume << std::endl;
}

void VolumeManager::decreaseVolume() {
    auto lVolume = volume;
    unsigned int newVolume = lVolume - VOLUME_STEP;

    if (newVolume >= MAX_VOLUME) //overflow condition
        lVolume = MIN_VOLUME;
    else
        lVolume = newVolume;

    volume = lVolume;
    std::cout << "decrease: " << lVolume << std::endl;
}
