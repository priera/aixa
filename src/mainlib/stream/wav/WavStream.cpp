#include "WavStream.h"

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

}
