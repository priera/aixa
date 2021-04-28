#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H

#include <mainlib/stream/Stream.h>

#include <QStringList>
#include <memory>

#include "AudioDefinitions.h"
#include "AudioWorker.h"

using namespace aixa::math;

class AudioWorkerFactory {
public:
    AudioWorkerFactory() = default;

    AudioWorker *buildWithInputStream(const std::string &streamPath);

    static const QStringList &acceptedFormats() {
        static QStringList acceptedStreamFormats = {"wav", "mp3"};
        return acceptedStreamFormats;
    }

private:
    std::shared_ptr<Stream> tryToGetStream(const std::string &streamPath);

    AudioEnvironment setupAudioEnvironment(AudioStreamParameters &streamParams);

    int setHwParams(AlsaEnvironment &env, const AudioStreamParameters &parameters);

    int setSwParams(AlsaEnvironment &environment, const AudioStreamParameters &parameters);
};

#endif  // AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H
