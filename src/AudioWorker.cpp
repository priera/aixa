#include "AudioWorker.h"


#include <sstream>
#include <exception>

#include <iostream>

AudioWorker::AudioWorker() :
        stopValue(false)
    {}

/*void AudioWorker::setup() {
    std::cout << "Playback device is " << device << "\n";
    std::cout << "Stream parameters are " << rate << "Hz, " << snd_pcm_format_name(format) << ", " << channels << " channels\n";
    std::cout << "Sine wave rate is " << freq << "Hz\n";

    int err;
    std::stringstream s;

    if ((err = snd_output_stdio_attach(&output, stdout, 0)) < 0) {
        s << "Output failed: " << snd_strerror(err) << "\n";
        throw std::runtime_error(s.str());
    }

    if ((err = snd_pcm_open(&handle, device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        s << "Playback open error: " << snd_strerror(err) << "\n";
        throw std::runtime_error(s.str());
    }

    if ((err = setHwParams(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        s << "Setting of hwparams failed: " << snd_strerror(err) << "\n";
        throw std::runtime_error(s.str());
    }
    if ((err = setSwParams(handle, swparams)) < 0) {
        s << "Setting of swparams failed: " << snd_strerror(err) << "\n";
        throw std::runtime_error(s.str());
    }

    snd_pcm_dump(handle, output);
}

int AudioWorker::setHwParams(snd_pcm_t *handle,
                snd_pcm_hw_params_t *params,
                snd_pcm_access_t access) {

    unsigned int rrate;
    snd_pcm_uframes_t size;
    int err, dir;

    /* choose all parameters * /
    err = snd_pcm_hw_params_any(handle, params);
    if (err < 0) {
        printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
        return err;
    }

    /* set hardware resampling * /
    err = snd_pcm_hw_params_set_rate_resample(handle, params, 1);
    if (err < 0) {
        printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* set the interleaved read/write format * /
    err = snd_pcm_hw_params_set_access(handle, params, access);
    if (err < 0) {
        printf("Access type not available for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* set the sample format * /
    err = snd_pcm_hw_params_set_format(handle, params, format);
    if (err < 0) {
        printf("Sample format not available for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* set the count of channels * /
    err = snd_pcm_hw_params_set_channels(handle, params, channels);
    if (err < 0) {
        printf("Channels count (%u) not available for playbacks: %s\n", channels, snd_strerror(err));
        return err;
    }

    /* set the stream rate * /
    rrate = rate;
    err = snd_pcm_hw_params_set_rate_near(handle, params, &rrate, 0);
    if (err < 0) {
        printf("Rate %uHz not available for playback: %s\n", rate, snd_strerror(err));
        return err;
    }

    if (rrate != rate) {
        printf("Rate doesn't match (requested %uHz, get %iHz)\n", rate, err);
        return -EINVAL;
    }

    /* set the buffer time * /
    err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, &dir);
    if (err < 0) {
        printf("Unable to set buffer time %u for playback: %s\n", buffer_time, snd_strerror(err));
        return err;
    }

    err = snd_pcm_hw_params_get_buffer_size(params, &size);
    if (err < 0) {
        printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
        return err;
    }
    buffer_size = size;

    /* set the period time * /
    err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, &dir);
    if (err < 0) {
        printf("Unable to set period time %u for playback: %s\n", period_time, snd_strerror(err));
        return err;
    }

    err = snd_pcm_hw_params_get_period_size(params, &size, &dir);
    if (err < 0) {
        printf("Unable to get period size for playback: %s\n", snd_strerror(err));
        return err;
    }

    period_size = size;

    /* write the parameters to device * /
    err = snd_pcm_hw_params(handle, params);
    if (err < 0) {
        printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
        return err;
    }
    return 0;

}

int AudioWorker::setSwParams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams) {
    int err;

    /* get the current swparams * /
    err = snd_pcm_sw_params_current(handle, swparams);
    if (err < 0) {
        printf("Unable to determine current swparams for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* start the transfer when the buffer is almost full: * /
    /* (buffer_size / avail_min) * avail_min * /
    err = snd_pcm_sw_params_set_start_threshold(handle, swparams, (buffer_size / period_size) * period_size);
    if (err < 0) {
        printf("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* allow the transfer when at least period_size samples can be processed * /
    /* or disable this mechanism when period event is enabled (aka interrupt like style processing) * /
    err = snd_pcm_sw_params_set_avail_min(handle, swparams, period_size);
    if (err < 0) {
        printf("Unable to set avail min for playback: %s\n", snd_strerror(err));
        return err;
    }

    /* enable period events when requested * /
    /*
    if (period_event) {
        err = snd_pcm_sw_params_set_period_event(handle, swparams, 1);
        if (err < 0) {
            printf("Unable to set period event: %s\n", snd_strerror(err));
            return err;
        }
    } * /

    /* write the parameters to the playback device * /
    err = snd_pcm_sw_params(handle, swparams);
    if (err < 0) {
        printf("Unable to set sw params for playback: %s\n", snd_strerror(err));
        return err;
    }
    return 0;
}

 */

void AudioWorker::start() {
    while (!stopValue) {

    }
}

void AudioWorker::stop() {
    stopValue = true;
}

