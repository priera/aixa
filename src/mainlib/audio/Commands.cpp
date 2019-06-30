#include "Commands.h"

void VolumeUp::execute() {
    worker->increaseVolume();
}

std::string VolumeUp::getName() const {
    return "VolumeUp";
}


void VolumeDown::execute() {
    worker->decreaseVolume();
}

std::string VolumeDown::getName() const {
    return "VolumeDown";
}
