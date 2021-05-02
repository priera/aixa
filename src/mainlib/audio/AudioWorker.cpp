#include "AudioWorker.h"

#include "Commands.h"

AudioWorker::AudioWorker(std::unique_ptr<StreamReader> reader, std::unique_ptr<Publisher> publisher) :
    reader(std::move(reader)), publisher(std::move(publisher)), readingThread(*this->reader),
    publishingThread(*this->publisher) {
    auto volumeUp = new VolumeUp(*this);
    auto volumeDown = new VolumeDown(*this);

    myCommands.insert(std::make_pair(volumeUp->getName(), volumeUp));
    myCommands.insert(std::make_pair(volumeDown->getName(), volumeDown));
}

void AudioWorker::start() {
    readingThread.start();
    publishingThread.start();
}

void AudioWorker::stop() {
    readingThread.stopAndWait();
    publishingThread.stopAndWait();
}

void AudioWorker::increaseVolume() { publisher->increaseVolume(); }

void AudioWorker::decreaseVolume() { publisher->decreaseVolume(); }

CommandCollection AudioWorker::getCommandCollection() { return myCommands; }
