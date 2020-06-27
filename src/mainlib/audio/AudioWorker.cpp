#include "AudioWorker.h"

void AudioWorker::start() {
    readingThread.start();
    publishingThread.start();
}

void AudioWorker::stop() {
    readingThread.stopAndDetach();
    publishingThread.stopAndDetach();
}
