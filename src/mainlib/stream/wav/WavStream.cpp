#include "WavStream.h"

#include <cstring>

#include "WavFunctions.h"

AudioStreamParameters WavStream::getParameters() const {
    return AudioStreamParameters{SND_PCM_FORMAT_S16,
                                 format.samplingRate,
                                 format.channels,
                                 format.bytesPerSecond,
                                 format.bitsPerSample};
}

void WavStream::prepareForFirstRead() {
    unsigned int fileSize;

    WavFunctions::readHeader(f, fileSize);
    WavFunctions::skipUntilDataChunk(f);
}

void WavStream::storeSamples(InterleavedBuffer &buffer) {
    //FIXME Wav data is little endian. Mercifully, samples are stored in little endian in this computer too
    //  But if computer wasn't little endian, results would be wrong!!!

    auto toRead = buffer.dataSize();
    auto frame = buffer.frame();
    auto extracted = f.extractBytes(frame, toRead);

    if (extracted >= 0 && extracted < toRead) {
        std::memset(&frame[extracted], 0, toRead - extracted);
    }
}

bool WavStream::ended() {
    return f.ended();
}
