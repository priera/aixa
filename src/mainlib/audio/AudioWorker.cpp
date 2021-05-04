#include "AudioWorker.h"

#include "Commands.h"

AudioWorker::AudioWorker(std::unique_ptr<StreamReader> reader,
                         std::unique_ptr<AudioProcessingThread> processingThread) :
    reader(std::move(reader)),
    readingThread(*this->reader), processingThread(std::move(processingThread)) {
    auto volumeUp = new VolumeUp(*this);
    auto volumeDown = new VolumeDown(*this);

    myCommands.insert(std::make_pair(volumeUp->getName(), volumeUp));
    myCommands.insert(std::make_pair(volumeDown->getName(), volumeDown));
}

void AudioWorker::start() {
    readingThread.start();
    processingThread->start();
}

void AudioWorker::stop() {
    readingThread.stopAndWait();
    // publishingThread.stopAndWait();
    processingThread->requestInterruption();
}

void AudioWorker::increaseVolume() { processingThread->increaseVolume(); }

void AudioWorker::decreaseVolume() { processingThread->decreaseVolume(); }

CommandCollection AudioWorker::getCommandCollection() { return myCommands; }
