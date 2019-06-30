#include "AudioBuilder.h"

#include <iostream>
#include <sstream>

void pedroFunction() {

}

AudioEnvironment *AudioBuilder::setupAudioEnvironment(const AudioParameters &parameters) {
    AlsaEnvironment environment;
    snd_pcm_hw_params_alloca(&environment.hwparams);
    snd_pcm_sw_params_alloca(&environment.swparams);

    std::cout << "Playback device is " << parameters.device << "\n";
    std::cout << "Stream parameters are " << parameters.rate << "Hz, " << snd_pcm_format_name(parameters.format) << ", " << parameters.channels << " channels\n";
    std::cout << "Sine wave rate is " << parameters.freq << "Hz\n";

    int err;
    std::stringstream s;

    if ((err = snd_output_stdio_attach(&environment.output, stdout, 0)) < 0) {
        s << "Output failed: " << snd_strerror(err) << "\n";
        throw std::runtime_error(s.str());
    }

    if ((err = snd_pcm_open(&environment.handle, parameters.device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        s << "Playback open error: " << snd_strerror(err) << "\n";
        throw std::runtime_error(s.str());
    }

    if ((err = setHwParams(environment, SND_PCM_ACCESS_RW_INTERLEAVED, parameters)) < 0) {
        s << "Setting of hwparams failed: " << snd_strerror(err) << "\n";
        throw std::runtime_error(s.str());
    }
    if ((err = setSwParams(environment, parameters)) < 0) {
        s << "Setting of swparams failed: " << snd_strerror(err) << "\n";
        throw std::runtime_error(s.str());
    }

    snd_pcm_dump(environment.handle, environment.output);

    Buffers buffers(parameters.channels, environment.frame_size, parameters.format);

    auto ret = new AudioEnvironment(parameters, environment, buffers);
    return ret;
}

int AudioBuilder::setHwParams(AlsaEnvironment &environment,
                              snd_pcm_access_t access,
                              const AudioParameters &parameters) {

    snd_pcm_t *handle = environment.handle;
    snd_pcm_hw_params_t * params = environment.hwparams;

    unsigned int rrate;
    snd_pcm_uframes_t size;
    int err, dir;

    /* choose all parameters */
    err = snd_pcm_hw_params_any(handle, params);
    if (err < 0) {
        printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
        return err;
    }

    /* set hardware resampling */
    err = snd_pcm_hw_params_set_rate_resample(handle, params, 1);
    if (err < 0) {
        printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* set the interleaved read/write format */
    err = snd_pcm_hw_params_set_access(handle, params, access);
    if (err < 0) {
        printf("Access type not available for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* set the sample format */
    err = snd_pcm_hw_params_set_format(handle, params, parameters.format);
    if (err < 0) {
        printf("Sample format not available for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* set the count of channels */
    err = snd_pcm_hw_params_set_channels(handle, params, parameters.channels);
    if (err < 0) {
        printf("Channels count (%u) not available for playbacks: %s\n", parameters.channels, snd_strerror(err));
        return err;
    }

    /* set the stream rate */
    rrate = parameters.rate;
    err = snd_pcm_hw_params_set_rate_near(handle, params, &rrate, 0);
    if (err < 0) {
        printf("Rate %uHz not available for playback: %s\n", parameters.rate, snd_strerror(err));
        return err;
    }

    if (rrate != parameters.rate) {
        printf("Rate doesn't match (requested %uHz, get %iHz)\n", parameters.rate, err);
        return -EINVAL;
    }

    /* set the buffer time */
    err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &parameters.buffer_time, &dir);
    if (err < 0) {
        printf("Unable to set buffer time %u for playback: %s\n", parameters.buffer_time, snd_strerror(err));
        return err;
    }

    err = snd_pcm_hw_params_get_buffer_size(params, &size);
    if (err < 0) {
        printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
        return err;
    }
    environment.buffer_size = size;

    /* set the period time */
    err = snd_pcm_hw_params_set_period_time_near(handle, params, &parameters.period_time, &dir);
    if (err < 0) {
        printf("Unable to set period time %u for playback: %s\n", parameters.period_time, snd_strerror(err));
        return err;
    }

    err = snd_pcm_hw_params_get_period_size(params, &size, &dir);
    if (err < 0) {
        printf("Unable to get period size for playback: %s\n", snd_strerror(err));
        return err;
    }

    environment.frame_size = size;

    /* write the parameters to device */
    err = snd_pcm_hw_params(handle, params);
    if (err < 0) {
        printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
        return err;
    }
    return 0;
}

int AudioBuilder::setSwParams(AlsaEnvironment &environment,
                              const AudioParameters &parameters) {

    snd_pcm_t *handle = environment.handle;
    snd_pcm_sw_params_t *swparams = environment.swparams;

    int err;
    /* get the current swparams */
    err = snd_pcm_sw_params_current(handle, swparams);
    if (err < 0) {
        printf("Unable to determine current swparams for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* start the transfer when the buffer is almost full: */
    /* (buffer_size / avail_min) * avail_min */
    err = snd_pcm_sw_params_set_start_threshold(handle, swparams, (environment.buffer_size / environment.frame_size) * environment.frame_size);
    if (err < 0) {
        printf("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* allow the transfer when at least frame_size samples can be processed */
    /* or disable this mechanism when period event is enabled (aka interrupt like style processing) */
    err = snd_pcm_sw_params_set_avail_min(handle, swparams, environment.frame_size);
    if (err < 0) {
        printf("Unable to set avail min for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* enable period events when requested */
    /*
    if (period_event) {
        err = snd_pcm_sw_params_set_period_event(handle, swparams, 1);
        if (err < 0) {
            printf("Unable to set period event: %s\n", snd_strerror(err));
            return err;
        }
    } */

    /* write the parameters to the playback device */
    err = snd_pcm_sw_params(handle, swparams);
    if (err < 0) {
        printf("Unable to set sw params for playback: %s\n", snd_strerror(err));
        return err;
    }
    return 0;
}
