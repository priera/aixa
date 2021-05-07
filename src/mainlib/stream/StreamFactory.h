#ifndef AIXA_SRC_MAINLIB_STREAM_STREAMFACTORY_H
#define AIXA_SRC_MAINLIB_STREAM_STREAMFACTORY_H

#include <memory>

#include "Stream.h"

class StreamFactory {
public:
    virtual ~StreamFactory() = default;

    virtual std::shared_ptr<Stream> probe() = 0;
};

#endif //AIXA_SRC_MAINLIB_STREAM_STREAMFACTORY_H
