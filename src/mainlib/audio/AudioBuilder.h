#ifndef AIXA_AUDIOBUILDER_H
#define AIXA_AUDIOBUILDER_H

#include "AudioDefinitions.h"

class AudioBuilder {
public:
    AudioBuilder() = default;

    AudioEnvironment *setupAudioEnvironment(AudioParameters &parameters);

private:
    int setHwParams(AlsaEnvironment &environment,
                    snd_pcm_access_t access,
                    AudioParameters &parameters);

    int setSwParams(AlsaEnvironment &environment,
                    const AudioParameters &parameters);
};

#endif //AIXA_AUDIOBUILDER_H
