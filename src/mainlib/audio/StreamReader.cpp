#include "StreamReader.h"

void StreamReader::exec() {
    if (!initialized) {
        stream->prepareForFirstRead();
        initialized = true;
    }

    auto buffer = samplesRing->nextWriteBuffer();
    buffer->startNewFrame();

    //This looks inefficient as hell. Something not so decoupled would perform better
    //You know. This is not Java. We're trying to get something done here
    size_t samplesCount = buffer->dataSize();
    for (size_t i = 0; i < samplesCount; i++) {
        buffer->storeNextSample(stream->nextSample());
    }

}
