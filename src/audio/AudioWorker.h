#ifndef ALSAPLAYGROUND_AUDIOWORKER_H
#define ALSAPLAYGROUND_AUDIOWORKER_H

#include <atomic>
#include <string>
#include <memory>

#include <alsa/asoundlib.h>

#include "CommandBuilder.h"

#include "audio/AudioDefinitions.h"

class NoteSetter;

class AudioWorker : public CommandBuilder {
public:
    AudioWorker(std::unique_ptr<AudioEnvironment> &environment);

    CommandCollection buildCommandCollection() override;

    NoteSetter *getNoteSetter() const;

    void start();
    void stop();

    void increaseVolume();
    void decreaseVolume();

    void setFrequency(double freq);

private:
    static constexpr unsigned int MIN_VOLUME = 0;

    static constexpr int FORMAT_BITS = 16; //snd_pcm_format_width(SND_PCM_FORMAT_S16);
    static constexpr unsigned int MAX_VOLUME = (1 << (FORMAT_BITS - 1)) - 1;
    static constexpr unsigned int VOLUME_STEP = (MAX_VOLUME - MIN_VOLUME) / 20;

    void writeLoop();

    std::unique_ptr<AudioEnvironment> environment;

    std::atomic<unsigned int> volume;
    std::atomic<double> freq;

    CommandCollection myCommands;
    std::unique_ptr<NoteSetter> noteSetter;

    std::atomic<bool> stopValue;
};


#endif //ALSAPLAYGROUND_AUDIOWORKER_H
