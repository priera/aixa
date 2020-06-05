#include "AudioWorker.h"

#include <sstream>
#include <exception>

#include <iostream>

#include "Commands.h"
#include "SineGenerator.h"

AudioWorker::AudioWorker(std::unique_ptr<AudioEnvironment> &paramEnvironment) :
    freq(0),
    environment(std::move(paramEnvironment)),
    volume(MAX_VOLUME/3),
    stopValue(false)
{
    auto volumeUp = new VolumeUp(*this);
    auto volumeDown = new VolumeDown(*this);

    myCommands.insert(std::make_pair(volumeUp->getName(), volumeUp));
    myCommands.insert(std::make_pair(volumeDown->getName(), volumeDown));

    sineGenerator = std::make_unique<SineGenerator>(environment->buffers, environment->platform.frame_size, environment->params.rate);
}

AudioWorker::~AudioWorker() {
}

void AudioWorker::increaseVolume() {
    volume += VOLUME_STEP;
    if (volume > MAX_VOLUME) volume = MAX_VOLUME;
}

void AudioWorker::decreaseVolume() {
    unsigned int newVolume = volume - VOLUME_STEP;

    if (newVolume >= MAX_VOLUME) //overflow condition
        volume = MIN_VOLUME;
    else
        volume = newVolume;
}

CommandCollection AudioWorker::buildCommandCollection() {
    return myCommands;
}

void AudioWorker::notifyNewValue(const Note& newNote) {
    freq = computeFrequency(newNote);
}

void AudioWorker::start() {
    while (!stopValue) {
        writeLoop();
    }
}

void AudioWorker::stop() {
    stopValue = true;
}

void AudioWorker::writeLoop() {
    int err, framesToWrite;

    sineGenerator->fillFrame(freq, volume);

    framesToWrite = environment->platform.frame_size;

    while (framesToWrite > 0) {
        //TODO: handle the case when there is more than one channel
        err = snd_pcm_writei(environment->platform.handle, environment->buffers.frame(), framesToWrite);
        if (err < 0)
            attemptStreamRecovery(err);

        framesToWrite -= err;
    }
}

void AudioWorker::attemptStreamRecovery(int err) {
    //Other possible error codes than under-run not considered yet
    if (err == -EPIPE) {    /* under-run */
        err = snd_pcm_prepare(environment->platform.handle);
        if (err < 0) {
            std::stringstream s;
            s << snd_strerror(err);
            throw std::runtime_error(s.str());
        }
    }
}
