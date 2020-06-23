#include "AudioWorker.h"

#include <sstream>
#include <exception>

#include <iostream>

#include "Commands.h"
#include "SineGenerator.h"

#include <thread>

AudioWorker::AudioWorker(std::unique_ptr<AudioEnvironment> &paramEnvironment) :
    freq(0),
    environment(std::move(paramEnvironment)),
    sleepTime(std::chrono::microseconds(environment->params.period_time)),
    volume(MAX_VOLUME/3),
    stopValue(false)
{
    auto volumeUp = new VolumeUp(*this);
    auto volumeDown = new VolumeDown(*this);

    myCommands.insert(std::make_pair(volumeUp->getName(), volumeUp));
    myCommands.insert(std::make_pair(volumeDown->getName(), volumeDown));

    sineGenerator = std::make_unique<SineGenerator>(environment->samplesRing, environment->platform.frame_size, environment->params.rate);
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
    sineGenerator->fillFrame(freq, volume);

    waitForStream();

    int err, framesToWrite;
    framesToWrite = environment->platform.frame_size;

    auto buffer = environment->samplesRing->nextReadBuffer();

    while (framesToWrite > 0) {
        err = snd_pcm_writei(environment->platform.handle, buffer->frame(), framesToWrite);

        if (err < 0) {
            attemptStreamRecovery(err);
            break;
        }

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

void AudioWorker::waitForStream() {
    snd_pcm_sframes_t framesAvailable, frameSize;

    frameSize = environment->platform.frame_size;
    do {
        framesAvailable = snd_pcm_avail_update(environment->platform.handle);

        if (framesAvailable < frameSize) {
            if (framesAvailable < 0) {
                std::cout << "Error when checking stream state: " << snd_strerror(framesAvailable) << std::endl;
                attemptStreamRecovery(framesAvailable);
            } else {
                std::this_thread::sleep_for(sleepTime);
            }
        }
    } while (framesAvailable < frameSize);
}
