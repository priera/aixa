#ifndef AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAMFACTORY_H
#define AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAMFACTORY_H


#include <mainlib/stream/StreamFactory.h>

class WavStreamFactory : public StreamFactory {
public:
    WavStreamFactory(std::string streamPath) : filePath(std::move(streamPath)) {}

    ~WavStreamFactory() override = default;

    std::shared_ptr<Stream> probe() override;

private:
    std::string filePath;
};


#endif //AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAMFACTORY_H
