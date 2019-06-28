#include "AudioWorker.h"

#include <math.h>

#include <sstream>
#include <exception>

#include <iostream>

static void generate_sine(const snd_pcm_channel_area_t *areas,
                          snd_pcm_uframes_t offset,
                          int count,
                          double *_phase,
                          double freq,
                          double rate,
                          unsigned int channels) {
    //count (period size) = 4410
    static double max_phase = 2. * M_PI;
    double phase = *_phase;
    double step = max_phase * freq / rate;
    unsigned char *samples[channels];
    int steps[channels];
    unsigned int chn;
    int format_bits = snd_pcm_format_width(SND_PCM_FORMAT_S16); // 16
    unsigned int maxval = (1 << (format_bits - 1)) - 1; // 32767
    int bps = format_bits / 8;  /* bytes per sample */
    int phys_bps = snd_pcm_format_physical_width(SND_PCM_FORMAT_S16) / 8;
    int big_endian = snd_pcm_format_big_endian(SND_PCM_FORMAT_S16) == 1;
    int to_unsigned = snd_pcm_format_unsigned(SND_PCM_FORMAT_S16) == 1;
    //int is_float = (SND_PCM_FORMAT_S16 == SND_PCM_FORMAT_FLOAT_LE ||
    //        SND_PCM_FORMAT_S16 == SND_PCM_FORMAT_FLOAT_BE);

    //std::cout << big_endian << " " << bps << std::endl;
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
    while (count-- > 0) {
        int res, i;

        res = sin(phase) * maxval;

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
    environment(std::move(environment)),
    stopValue(false)
    {}

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

    double freq = params.freq;
    double rate = params.rate;

    generate_sine(environment->areas,
            0,
            environment->period_size,
            &phase,
            freq,
            rate,
            params.channels);

    cptr = environment->period_size;

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
