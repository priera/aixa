#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H

#include <memory>

#include <mainlib/stream/Stream.h>

#include "AudioDefinitions.h"
#include "AudioWorker.h"

class AudioWorkerFactory {
public:
    std::unique_ptr<AudioWorker> buildWithInputStream(const std::string &streamPath);

private:
    std::shared_ptr<Stream> tryToGetStream(const std::string &streamPath);

    AudioEnvironment setupAudioEnvironment(AudioStreamParameters &streamParams);

    int setHwParams(AlsaEnvironment &env,
                    snd_pcm_access_t access,
                    const AudioStreamParameters &parameters);

    int setSwParams(AlsaEnvironment &environment,
                    const AudioStreamParameters &parameters);
};


#endif //AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H
