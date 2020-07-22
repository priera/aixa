#include "WavStream.h"

#include <cstring>

#include "WavFunctions.h"

AudioStreamParameters WavStream::getParameters() const {
    auto sampleFormat = SND_PCM_FORMAT_S16;

    return AudioStreamParameters{sampleFormat,
                                 snd_pcm_format_little_endian(sampleFormat) == 1,
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
    auto toRead = buffer.dataSize();
    auto frame = buffer.raw();
    auto extracted = f.extractBytes(frame, toRead);

    if (extracted >= 0 && extracted < toRead) {
        std::memset(&frame[extracted], 0, toRead - extracted);
    }
}

bool WavStream::ended() {
    return f.ended();
}
