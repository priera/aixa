#ifndef ALSAPLAYGROUND_AUDIOWORKER_H
#define ALSAPLAYGROUND_AUDIOWORKER_H

#include <atomic>
#include <string>

#include <alsa/asoundlib.h>

class AudioWorker {
public:
    AudioWorker();

    void start();
    void stop();

private:
    void setup();

    int setHwParams(snd_pcm_t *handle,
                     snd_pcm_hw_params_t *params,
                     snd_pcm_access_t access);

    int setSwParams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams);

    std::atomic<bool> stopValue;

    const std::string device;   /* playback device */
    snd_pcm_format_t format;    /* sample format */
    unsigned int rate;          /* stream rate */
    unsigned int channels;      /* count of channels */
    unsigned int buffer_time;   /* ring buffer length in us */
    unsigned int period_time;   /* period time in us */
    double freq;

    snd_pcm_t *handle;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_sw_params_t *swparams;
    snd_output_t *output;
};


#endif //ALSAPLAYGROUND_AUDIOWORKER_H
