#include "AudioWorkerFactory.h"

#include <mainlib/globals.h>
#include <mainlib/math/dft/FourierTransformFactory.h>
#include <mainlib/math/dft/SpectrogramBuilder.h>
#include <mainlib/stream/StreamTypes.h>

#include <QAudioDeviceInfo>
#include <iostream>

#include "InterleavedBufferGenerator.h"

using namespace aixa::math;

AudioWorker *AudioWorkerFactory::buildWithInputStream(const std::string &streamPath) {
    auto stream = tryToGetStream(streamPath);

    if (!stream) {
        return nullptr;
    }

    auto streamParams = stream->getParameters();
    auto environment = setupAudioEnvironment(streamParams);

    auto streamReader = std::make_unique<StreamReader>(stream, environment.samplesRing);
    auto volumeManager = std::make_unique<VolumeManager>();

    double sampleRate = 1.0 / streamParams.rate;
    auto spectrogramComputer_p = SpectrogramBuilder(sampleRate).build(USE_LOG_SCALES);
    auto spectrogramComputer = std::unique_ptr<SpectrogramComputer>(spectrogramComputer_p);

    auto publisher = std::make_unique<Publisher>(environment.platform, environment.samplesRing,
                                                 std::move(volumeManager), std::move(spectrogramComputer));

    return new AudioWorker(std::move(streamReader), std::move(publisher));
}

std::shared_ptr<Stream> AudioWorkerFactory::tryToGetStream(const std::string &streamPath) {
    auto ret = std::shared_ptr<Stream>(nullptr);

    try {
        ret = StreamTypes::getMatchingStreamFactory(streamPath)->probe();
    } catch (std::exception &e) {
        std::cerr << "Could not obtain stream due to error: " << e.what() << std::endl;
    }

    return ret;
}

AudioEnvironment AudioWorkerFactory::setupAudioEnvironment(AudioStreamParameters &streamParams) {
    AlsaEnvironment alsaEnv;
    alsaEnv.params = AlsaParameters{"default", 500000, 100000};

    if (!streamParams.littleEndianSamples)
        throw std::runtime_error("Stream format not supported: samples are not in little-endian");

    QAudioFormat format;
    format.setSampleRate(streamParams.rate);
    format.setChannelCount(streamParams.channels);
    format.setSampleSize(streamParams.bitsSample);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    auto device = QAudioDeviceInfo::defaultOutputDevice();
    if (!device.isFormatSupported(format)) {
        std::cerr << "Your device does not support required stream format" << std::endl;
        format = device.nearestFormat(format);
    }

    /* Notice the asymmetry: internally ALSA stores five buffers (500000 us of audio)
     * while buffersRing stores one second */
    const auto BUFFERS_FOR_ONE_SECOND = 1000000 / alsaEnv.params.period_time;
    auto bufferGenerator =
        InterleavedBufferGenerator(streamParams.channels, alsaEnv.frame_size, streamParams.format);
    auto samplesRing = std::make_shared<SamplesRing>(BUFFERS_FOR_ONE_SECOND, bufferGenerator.generator());

    return AudioEnvironment(streamParams, alsaEnv, samplesRing);
}

// AudioEnvironment AudioWorkerFactory::setupAudioEnvironment(AudioStreamParameters &streamParams) {
//    AlsaEnvironment alsaEnv;
//    alsaEnv.params = AlsaParameters{"default", 500000, 100000};
//
//    if (!streamParams.littleEndianSamples)
//        throw std::runtime_error("Not little endian streams not supported");
//
//    snd_pcm_hw_params_alloca(&alsaEnv.hwparams);
//    snd_pcm_sw_params_alloca(&alsaEnv.swparams);
//
//    std::cout << "Playback device is " << alsaEnv.params.device << "\n";
//    std::cout << "Stream streamParams are " << streamParams.rate << "Hz, "
//              << snd_pcm_format_name(streamParams.format) << ", " << streamParams.channels << " channels\n";
//
//    int err;
//    std::stringstream s;
//
//    if ((err = snd_output_stdio_attach(&alsaEnv.output, stdout, 0)) < 0) {
//        s << "Output failed: " << snd_strerror(err) << "\n";
//        throw std::runtime_error(s.str());
//    }
//
//    if ((err = snd_pcm_open(&alsaEnv.handle, alsaEnv.params.device.c_str(), SND_PCM_STREAM_PLAYBACK, 0)) <
//        0) {
//        s << "Playback open error: " << snd_strerror(err) << "\n";
//        throw std::runtime_error(s.str());
//    }
//
//    if ((err = setHwParams(alsaEnv, streamParams)) < 0) {
//        s << "Setting of hwparams failed: " << snd_strerror(err) << "\n";
//        throw std::runtime_error(s.str());
//    }
//    if ((err = setSwParams(alsaEnv, streamParams)) < 0) {
//        s << "Setting of swparams failed: " << snd_strerror(err) << "\n";
//        throw std::runtime_error(s.str());
//    }
//
//    snd_pcm_dump(alsaEnv.handle, alsaEnv.output);
//
//    /* Notice the asymmetry: internally ALSA stores five buffers (500000 us of audio)
//     * while buffersRing stores one second */
//    const auto BUFFERS_FOR_ONE_SECOND = 1000000 / alsaEnv.params.period_time;
//    auto bufferGenerator =
//        InterleavedBufferGenerator(streamParams.channels, alsaEnv.frame_size, streamParams.format);
//    auto samplesRing = std::make_shared<SamplesRing>(BUFFERS_FOR_ONE_SECOND, bufferGenerator.generator());
//
//    return AudioEnvironment(streamParams, alsaEnv, samplesRing);
//}

// int AudioWorkerFactory::setHwParams(AlsaEnvironment &env, const AudioStreamParameters &parameters) {
//    snd_pcm_t *handle = env.handle;
//    snd_pcm_hw_params_t *params = env.hwparams;
//
//    unsigned int rrate;
//    snd_pcm_uframes_t size;
//    int err, dir;
//
//    /* choose all parameters */
//    err = snd_pcm_hw_params_any(handle, params);
//    if (err < 0) {
//        printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
//        return err;
//    }
//
//    /* set hardware resampling */
//    err = snd_pcm_hw_params_set_rate_resample(handle, params, 1);
//    if (err < 0) {
//        printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//
//    /* set the interleaved storeNextSamplesAt/write format */
//    err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
//    if (err < 0) {
//        printf("Access type not available for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//
//    /* set the sample format */
//    err = snd_pcm_hw_params_set_format(handle, params, parameters.format);
//    if (err < 0) {
//        printf("Sample format not available for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//
//    /* set the count of channels */
//    err = snd_pcm_hw_params_set_channels(handle, params, parameters.channels);
//    if (err < 0) {
//        printf("Channels count (%u) not available for playbacks: %s\n", parameters.channels,
//               snd_strerror(err));
//        return err;
//    }
//
//    /* set the stream rate */
//    rrate = parameters.rate;
//    err = snd_pcm_hw_params_set_rate_near(handle, params, &rrate, nullptr);
//    if (err < 0) {
//        printf("Rate %uHz not available for playback: %s\n", parameters.rate, snd_strerror(err));
//        return err;
//    }
//
//    if (rrate != parameters.rate) {
//        printf("Rate doesn't match (requested %uHz, get %iHz)\n", parameters.rate, err);
//        return -EINVAL;
//    }
//
//    /* set the buffer time */
//    err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &env.params.buffer_time, &dir);
//    if (err < 0) {
//        printf("Unable to set buffer time %u for playback: %s\n", env.params.buffer_time,
//        snd_strerror(err)); return err;
//    }
//
//    err = snd_pcm_hw_params_get_buffer_size(params, &size);
//    if (err < 0) {
//        printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//    env.buffer_size = size;
//
//    /* set the period time */
//    err = snd_pcm_hw_params_set_period_time_near(handle, params, &env.params.period_time, &dir);
//    if (err < 0) {
//        printf("Unable to set period time %u for playback: %s\n", env.params.period_time,
//        snd_strerror(err)); return err;
//    }
//
//    err = snd_pcm_hw_params_get_period_size(params, &size, &dir);
//    if (err < 0) {
//        printf("Unable to get period size for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//
//    env.frame_size = size;
//
//    /* write the parameters to device */
//    err = snd_pcm_hw_params(handle, params);
//    if (err < 0) {
//        printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//    return 0;
//}
//
// int AudioWorkerFactory::setSwParams(AlsaEnvironment &environment, const AudioStreamParameters &parameters)
// {
//    snd_pcm_t *handle = environment.handle;
//    snd_pcm_sw_params_t *swparams = environment.swparams;
//
//    int err;
//    /* get the current swparams */
//    err = snd_pcm_sw_params_current(handle, swparams);
//    if (err < 0) {
//        printf("Unable to determine current swparams for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//
//    /* start the transfer when the buffer is almost full: */
//    /* (buffer_size / avail_min) * avail_min */
//    err = snd_pcm_sw_params_set_start_threshold(
//        handle, swparams, (environment.buffer_size / environment.frame_size) * environment.frame_size);
//    if (err < 0) {
//        printf("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//
//    /* allow the transfer when at least frame_size samplesRing can be processed */
//    /* or disable this mechanism when period event is enabled (aka interrupt like style processing) */
//    err = snd_pcm_sw_params_set_avail_min(handle, swparams, environment.frame_size);
//    if (err < 0) {
//        printf("Unable to set avail min for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//
//    /* enable period events when requested */
//    /*
//    if (period_event) {
//        err = snd_pcm_sw_params_set_period_event(handle, swparams, 1);
//        if (err < 0) {
//            printf("Unable to set period event: %s\n", snd_strerror(err));
//            return err;
//        }
//    } */
//
//    /* write the parameters to the playback device */
//    err = snd_pcm_sw_params(handle, swparams);
//    if (err < 0) {
//        printf("Unable to set sw params for playback: %s\n", snd_strerror(err));
//        return err;
//    }
//    return 0;
//}