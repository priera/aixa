#include "StreamReader.h"

void StreamReader::exec() {
    if (!initialized) {
        stream->prepareForFirstRead();
        initialized = true;
    }

    auto buffer = samplesRing->nextWriteBuffer();
    buffer->startNewFrame();

    stream->storeSamples(*buffer);
}
