#include "WavStream.h"

#include <cstring>

#include "WavFunctions.h"

QAudioFormat WavStream::getParameters() const {
    QAudioFormat ret;
    ret.setSampleRate(format.samplingRate);
    ret.setChannelCount(format.channels);
    ret.setSampleSize(format.bitsPerSample);
    ret.setCodec("audio/pcm");
    ret.setByteOrder(QAudioFormat::LittleEndian);
    ret.setSampleType(QAudioFormat::SignedInt);
    return ret;
}

void WavStream::prepareForFirstRead() {
    unsigned int fileSize;

    WavFunctions::readHeader(*f, fileSize);
    WavFunctions::skipUntilDataChunk(*f);
}

void WavStream::storeSamples(InterleavedBuffer &buffer) {
    auto toRead = buffer.dataSize();
    auto frame = buffer.raw();
    auto extracted = f->extractBytes(frame, toRead);

    if (extracted >= 0 && extracted < toRead) {
        std::memset(&frame[extracted], 0, toRead - extracted);
    }
}

bool WavStream::ended() { return f->ended(); }
