#ifndef ALSAPLAYGROUND_AUDIOWORKER_H
#define ALSAPLAYGROUND_AUDIOWORKER_H

#include <atomic>
#include <string>
#include <memory>

#include <alsa/asoundlib.h>

#include "audio/AudioDefinitions.h"

#include "CommandBuilder.h"

class AudioWorker : public CommandBuilder {
public:
    AudioWorker(const AudioParameters &params, std::unique_ptr<AlsaEnvironment> &environment);

    CommandCollection buildCommandCollection() override;

    void start();
    void stop();

    void increaseVolume();
    void decreaseVolume();

private:
    static constexpr unsigned int MIN_VOLUME = 0;

    static constexpr int FORMAT_BITS = 16; //snd_pcm_format_width(SND_PCM_FORMAT_S16);
    static constexpr unsigned int MAX_VOLUME = (1 << (FORMAT_BITS - 1)) - 1;
    static constexpr unsigned int VOLUME_STEP = (MAX_VOLUME - MIN_VOLUME) / 20;

    void writeLoop();

    AudioParameters params;
    std::unique_ptr<AlsaEnvironment> environment;

    std::atomic<unsigned int> volume;

    CommandCollection myCommands;

    std::atomic<bool> stopValue;
};


#endif //ALSAPLAYGROUND_AUDIOWORKER_H
