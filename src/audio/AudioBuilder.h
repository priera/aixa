#ifndef ALSAPLAYGROUND_AUDIOBUILDER_H
#define ALSAPLAYGROUND_AUDIOBUILDER_H

#include "AudioDefinitions.h"

class AudioBuilder {
public:
    AudioBuilder() = default;

    AlsaEnvironment * setupAudioEnvironment(const AudioParameters &parameters);

private:
    int setHwParams(AlsaEnvironment &environment,
                    snd_pcm_access_t access,
                    const AudioParameters &parameters);

    int setSwParams(AlsaEnvironment &environment,
                    const AudioParameters &parameters);

    void setupMemory(AlsaEnvironment &environment,
        const AudioParameters & parameters);
};

#endif //ALSAPLAYGROUND_AUDIOBUILDER_H
