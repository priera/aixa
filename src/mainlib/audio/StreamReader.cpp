#include "StreamReader.h"

void StreamReader::consume() {
    auto buffer = samplesRing->nextWriteBuffer();
    buffer->startNewFrame();

    size_t samplesCount = buffer->dataSize();
    for (size_t i = 0; i < samplesCount; i++) {
        buffer->storeNextSample(stream->nextSample());
    }

}
