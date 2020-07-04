#include "StreamReader.h"

bool StreamReader::exec() {
    bool done = false;

    if (!initialized) {
        /*FIXME: use stream parameters to check endianness matching*/
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
