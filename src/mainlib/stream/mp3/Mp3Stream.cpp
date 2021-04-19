#include "Mp3Stream.h"

AudioStreamParameters Mp3Stream::getParameters() const { return AudioStreamParameters(); }

void Mp3Stream::prepareForFirstRead() { throw std::runtime_error("Not implemented yet"); }

bool Mp3Stream::ended() {
    throw std::runtime_error("Not implemented yet");
    return false;
}

void Mp3Stream::storeSamples(InterleavedBuffer& buffer) { throw std::runtime_error("Not implemented yet"); }
