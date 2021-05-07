#ifndef AIXA_SRC_MAINLIB_STREAM_STREAMTYPES_H
#define AIXA_SRC_MAINLIB_STREAM_STREAMTYPES_H

#include "StreamFactory.h"

class StreamTypes {
public:
    static std::unique_ptr<StreamFactory> getMatchingStreamFactory(const std::string &streamPath);

private:
    static std::string validateFileStreamPath(const std::string &streamPath);
};

#endif  // AIXA_SRC_MAINLIB_STREAM_STREAMTYPES_H
