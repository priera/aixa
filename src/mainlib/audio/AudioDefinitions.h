#ifndef AIXA_AUDIODEFINITIONS_H
#define AIXA_AUDIODEFINITIONS_H

#include <string>
#include <utility>
#include <vector>

#include <alsa/asoundlib.h>
#include <mainlib/buffers/BuffersRing.h>

#include "InterleavedBuffer.h"

struct AudioParameters {
    std::string device;   /* playback device */
    snd_pcm_format_t format;    /* sample format */
    unsigned int rate;          /* stream rate */
    unsigned int channels;      /* count of channels */
    unsigned int buffer_time;   /* ring buffer length in us */
    unsigned int period_time;   /* period time in us */
    double freq;
};

inline AudioParameters getDefaultAudioParameters() {
    return { "default", SND_PCM_FORMAT_S16, 44100, 1, 500000, 100000, 440 };
}

struct AlsaEnvironment {
    //TODO: fix memory leaks

    snd_pcm_t *handle;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_sw_params_t *swparams;
    snd_output_t *output;

    snd_pcm_sframes_t buffer_size;
    snd_pcm_sframes_t frame_size;
};

using SamplesRing = BuffersRing<InterleavedBuffer>;

struct AudioEnvironment {
    AudioEnvironment(AudioParameters parameters, AlsaEnvironment &environment, std::shared_ptr<SamplesRing> ring) :
            params(std::move(parameters)),
            platform(environment),
            samplesRing(std::move(ring)) {}

    AudioParameters params;
    AlsaEnvironment platform;
    std::shared_ptr<SamplesRing> samplesRing;
};

#endif //AIXA_AUDIODEFINITIONS_H
