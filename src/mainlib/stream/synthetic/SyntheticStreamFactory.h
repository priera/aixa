#ifndef AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAMFACTORY_H
#define AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAMFACTORY_H


#include <mainlib/stream/StreamFactory.h>

class SyntheticStreamFactory : public StreamFactory {
public:
    SyntheticStreamFactory(const std::string& path) : StreamFactory() {}

    std::shared_ptr<Stream> probe() override;
};


#endif //AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAMFACTORY_H
