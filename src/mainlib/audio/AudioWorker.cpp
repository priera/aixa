#include "AudioWorker.h"

#include "Commands.h"

AudioWorker::AudioWorker(std::unique_ptr<StreamReader> reader,
                         std::unique_ptr<AudioProcessingThread> processingThread) :
    reader(std::move(reader)),
    readingThread(*this->reader), processingThread(std::move(processingThread)),
    processingThreadStopped(false) {
    auto volumeUp = new VolumeUp(*this);
    auto volumeDown = new VolumeDown(*this);

    myCommands.insert(std::make_pair(volumeUp->getName(), volumeUp));
    myCommands.insert(std::make_pair(volumeDown->getName(), volumeDown));

    QObject::connect(this->processingThread.get(), &QThread::finished, [this]() {
        processingThreadStopped = true;
        cvProcessingThread.notify_one();
    });
}

void AudioWorker::start() {
    readingThread.start();
    processingThread->start();
}

void AudioWorker::stop() {
    readingThread.stopAndWait();
    processingThread->requestInterruption();

    std::mutex m;
    std::unique_lock<std::mutex> l(m);
    cvProcessingThread.wait(l, [this]() { return processingThreadStopped; });
}

void AudioWorker::increaseVolume() { processingThread->increaseVolume(); }

void AudioWorker::decreaseVolume() { processingThread->decreaseVolume(); }

CommandCollection AudioWorker::getCommandCollection() { return myCommands; }
