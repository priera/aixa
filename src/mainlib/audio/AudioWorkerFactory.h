#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H

#include <aixa_export.h>
#include <mainlib/stream/Stream.h>

#include <QStringList>
#include <memory>

#include "AudioDefinitions.h"
#include "AudioWorker.h"

using namespace aixa::math;
using namespace std::chrono_literals;

class LIB_EXPORT AudioWorkerFactory {
public:
    AudioWorkerFactory() = default;

    AudioWorker *buildWithInputStream(const std::string &streamPath);

    static const QStringList &acceptedFormats() {
        static QStringList acceptedStreamFormats = {"wav", "mp3"};
        return acceptedStreamFormats;
    }

private:
    static constexpr auto PERIOD_TIME = 100000us;
    static constexpr auto BUFFERS_FOR_ONE_SECOND = 1s / PERIOD_TIME;

    using FPSeconds = std::chrono::duration<float, std::chrono::seconds::period>;

    static std::size_t computeFrameSize(int streamSampleRate, std::chrono::microseconds periodDuration) {
        auto periodInSeconds = FPSeconds(periodDuration).count();
        auto periodSize = static_cast<float>(streamSampleRate) * periodInSeconds;
        return static_cast<std::size_t>(periodSize);
    }

    std::shared_ptr<Stream> tryToGetStream(const std::string &streamPath);

    AudioEnvironment setupAudioEnvironment(QAudioFormat &format);
};

#endif  // AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKERFACTORY_H
