#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H


#include <memory>

class AudioWorker {
public:
    static std::unique_ptr<AudioWorker> runWith(const std::string &streamPath);

    virtual ~AudioWorker() = default;

private:
    AudioWorker() {

    }

};


#endif //AIXA_SRC_MAINLIB_AUDIO_AUDIOWORKER_H
