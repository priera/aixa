#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H

#include <memory>

#include <mainlib/stream/Stream.h>

#include "AudioDefinitions.h"
#include "AudioWorker.h"

class AudioWorkerFactory {
public:
    explicit AudioWorkerFactory(std::unique_ptr<aixa::math::SpectrogramComputer> spectrogramComputer) :
        spectrogramComputer(std::move(spectrogramComputer)) {}

    std::unique_ptr<AudioWorker> buildWithInputStream(const std::string &streamPath);

private:
    std::shared_ptr<Stream> tryToGetStream(const std::string &streamPath);

    AudioEnvironment setupAudioEnvironment(AudioStreamParameters &streamParams);

    int setHwParams(AlsaEnvironment &env,
                    const AudioStreamParameters &parameters);

    int setSwParams(AlsaEnvironment &environment,
                    const AudioStreamParameters &parameters);

    std::unique_ptr<aixa::math::SpectrogramComputer> spectrogramComputer;
};


#endif //AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H
