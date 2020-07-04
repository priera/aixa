#include "StreamReader.h"

bool StreamReader::exec() {
    bool done = false;

    if (!initialized) {
        stream->prepareForFirstRead();
        initialized = true;
    }

    if (!stream->ended()) {
        auto buffer = samplesRing->nextWriteBuffer();
        stream->storeSamples(*buffer);
    } else {
        samplesRing->doneWriting();
        done = true;
    }

    return done;
}
