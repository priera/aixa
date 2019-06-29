#include "AudioWorker.h"

#include <math.h>

#include <sstream>
#include <exception>

#include <iostream>

#include "audio/Commands.h"
#include "audio/NoteSetter.h"

static void generate_sine(const snd_pcm_channel_area_t *areas,
                          snd_pcm_uframes_t offset,
                          int frame_size,
                          double *_phase,
                          double freq,
                          double rate,
                          unsigned int channels,
                          unsigned int scaleFactor) {
    static double max_phase = 2. * M_PI;
    double phase = *_phase;

    double step = max_phase * freq / rate;
    unsigned char *samples[channels];
    int steps[channels];
    unsigned int chn;
    int format_bits = snd_pcm_format_width(SND_PCM_FORMAT_S16); // 16

    int bps = format_bits / 8;  /* bytes per sample */
    int phys_bps = snd_pcm_format_physical_width(SND_PCM_FORMAT_S16) / 8;
    int big_endian = snd_pcm_format_big_endian(SND_PCM_FORMAT_S16) == 1;
    int to_unsigned = snd_pcm_format_unsigned(SND_PCM_FORMAT_S16) == 1;

    /* verify and prepare the contents of areas */
    for (chn = 0; chn < channels; chn++) {
        if ((areas[chn].first % 8) != 0) {
            printf("areas[%u].first == %u, aborting...\n", chn, areas[chn].first);
            exit(EXIT_FAILURE);
        }
        samples[chn] = /*(signed short *)*/(((unsigned char *) areas[chn].addr) + (areas[chn].first / 8));
        if ((areas[chn].step % 16) != 0) {
            printf("areas[%u].step == %u, aborting...\n", chn, areas[chn].step);
            exit(EXIT_FAILURE);
        }
        steps[chn] = areas[chn].step / 8;
        samples[chn] += offset * steps[chn];
    }

    /* fill the channel areas */
    while (frame_size-- > 0) {
        int res, i;

        res = sin(phase) * scaleFactor;

        if (to_unsigned)
            res ^= 1U << (format_bits - 1);
        for (chn = 0; chn < channels; chn++) {
            /* Generate data in native endian format */
            if (big_endian) {
                for (i = 0; i < bps; i++)
                    *(samples[chn] + phys_bps - 1 - i) = (res >> i * 8) & 0xff;
            } else {
                for (i = 0; i < bps; i++)
                    *(samples[chn] + i) = (res >> i * 8) & 0xff;
            }
            samples[chn] += steps[chn];
        }
        phase += step;
        if (phase >= max_phase) {
            phase -= max_phase;
        }
    }

    *_phase = phase;
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

AudioWorker::AudioWorker(const AudioParameters &params, std::unique_ptr<AlsaEnvironment> &environment) :
    params(params),
    freq(params.freq),
    noteSetter(std::make_unique<NoteSetter>(*this)),
    environment(std::move(environment)),
    volume(MAX_VOLUME/3),
    stopValue(false)
{
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

    double rate = params.rate;

    generate_sine(environment->areas,
            0,
            environment->frame_size,
            &phase,
            freq,
            rate,
            params.channels,
            volume);

    cptr = environment->frame_size;

    while (cptr > 0) {
        err = snd_pcm_writei(environment->handle, environment->samples, cptr);
        if (err == -EAGAIN)
            continue;

        if (err < 0) {
            if (xrun_recovery(environment->handle, err) < 0) {
                printf("Write error: %s\n", snd_strerror(err));
                exit(EXIT_FAILURE);
            }
            break;
        }
        ptr += err * params.channels;
        cptr -= err;
    }
}
