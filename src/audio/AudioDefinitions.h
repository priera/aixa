#ifndef ALSAPLAYGROUND_AUDIODEFINITIONS_H
#define ALSAPLAYGROUND_AUDIODEFINITIONS_H

#include <string>

#include <alsa/asoundlib.h>

struct AudioParameters {
    const std::string device;   /* playback device */
    snd_pcm_format_t format;    /* sample format */
    unsigned int rate;          /* stream rate */
    unsigned int channels;      /* count of channels */
    unsigned int buffer_time;   /* ring buffer length in us */
    unsigned int period_time;   /* period time in us */
    double freq;
};

inline AudioParameters getDefaultAudioParameters() {
    return { "default", SND_PCM_FORMAT_S16, 44100, 2, 500000, 100000, 440 };
}

struct AlsaEnvironment {
    //TODO: fix memory leaks

    snd_pcm_t *handle;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_sw_params_t *swparams;
    snd_output_t *output;

    signed short *samples;
    snd_pcm_channel_area_t *areas;

    snd_pcm_sframes_t buffer_size;
    snd_pcm_sframes_t frame_size;
};

#endif //ALSAPLAYGROUND_AUDIODEFINITIONS_H