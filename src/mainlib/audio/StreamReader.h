#ifndef AIXA_SRC_MAINLIB_AUDIO_STREAMREADER_H
#define AIXA_SRC_MAINLIB_AUDIO_STREAMREADER_H

#include <mainlib/stream/Stream.h>

class StreamReader {
public:
    StreamReader(std::shared_ptr<Stream> stream, std::shared_ptr<SamplesRing> samplesRing) :
            stream(std::move(stream)),
            samplesRing(std::move(samplesRing)) {}

    virtual ~StreamReader() = default;

    void exec();

private:
    std::shared_ptr<Stream> stream;
    std::shared_ptr<SamplesRing> samplesRing;
    bool initialized{false};
};


#endif //AIXA_SRC_MAINLIB_AUDIO_STREAMREADER_H
