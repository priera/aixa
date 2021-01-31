#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAMFACTORY_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAMFACTORY_H

#include <mainlib/stream/StreamFactory.h>

class Mp3StreamFactory : public StreamFactory {
public:
    explicit Mp3StreamFactory(std::string streamPath) : streamPath(std::move(streamPath)) {}

    std::shared_ptr<Stream> probe() override;

private:
    std::string streamPath;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3STREAMFACTORY_H
