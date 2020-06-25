#ifndef AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAMFACTORY_H
#define AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAMFACTORY_H


#include <mainlib/stream/StreamFactory.h>

class WavStreamFactory : public StreamFactory {
public:
    WavStreamFactory(const std::string &streamPath);

    std::shared_ptr<Stream> probe() override;

};


#endif //AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAMFACTORY_H
