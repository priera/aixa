#include "AudioWorker.h"

#include <math.h>

#include <sstream>
#include <exception>

#include <iostream>

#include "audio/Commands.h"
#include "audio/NoteSetter.h"


static void generate_sine(
        Buffers &buffers,
        int frame_size,
        double &_phase,
        double freq,
        double rate,
        unsigned int scaleFactor) {
    static double max_phase = 2. * M_PI;
    double phase = _phase;

    double step = max_phase * freq / rate;

    buffers.startNewFrame();

    while (frame_size-- > 0) {
        int res;

        res = sin(phase) * scaleFactor;
        buffers.storeNextSample(res);

        phase += step;
        if (phase >= max_phase) {
            phase -= max_phase;
        }
    }

    _phase = phase;
}

static int xrun_recovery(snd_pcm_t *handle, int err) {
    std::cout << "stream recovery\n";

    if (err == -EPIPE) {    /* under-run */
        err = snd_pcm_prepare(handle);
        if (err < 0)
            printf("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
        return 0;
    } else if (err == -ESTRPIPE) {
        while ((err = snd_pcm_resume(handle)) == -EAGAIN)
            sleep(1);   /* wait until the suspend flag is released */
        if (err < 0) {
            err = snd_pcm_prepare(handle);
            if (err < 0)
                printf("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
        }
        return 0;
    }
    return err;
}

AudioWorker::AudioWorker(std::unique_ptr<AudioEnvironment> &environment) :
    freq(0),
    noteSetter(std::make_unique<NoteSetter>(*this)),
    environment(std::move(environment)),
    volume(MAX_VOLUME/3),
    stopValue(false)
{
    freq = this->environment->params.freq;

    auto volumeUp = new VolumeUp(*this);
    auto volumeDown = new VolumeDown(*this);

    myCommands.insert(std::make_pair(volumeUp->getName(), volumeUp));
    myCommands.insert(std::make_pair(volumeDown->getName(), volumeDown));
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

void AudioWorker::setFrequency(double freq) {
    this->freq = freq;
}

CommandCollection AudioWorker::buildCommandCollection() {
    return myCommands;
}

NoteSetter *AudioWorker::getNoteSetter() const {
    return noteSetter.get();
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
    double phase = 0;
    signed short *ptr;
    int err, cptr;

    double rate = environment->params.rate;

    generate_sine(environment->buffers,
            environment->platform.frame_size,
            phase,
            freq,
            rate,
            volume);

    cptr = environment->platform.frame_size;

    while (cptr > 0) {
        err = snd_pcm_writei(environment->platform.handle, environment->buffers.frame(), cptr);
        if (err == -EAGAIN)
            continue;

        if (err < 0) {
            if (xrun_recovery(environment->platform.handle, err) < 0) {
                printf("Write error: %s\n", snd_strerror(err));
                exit(EXIT_FAILURE);
            }
            break;
        }
        ptr += err * environment->params.channels;
        cptr -= err;
    }
}
