#include "FormatExtractor.h"

WavFormat FormatExtractor::readFormat(FileReader &reader, unsigned int size) {
    WavFormat ret{};

    if (size < 16)
        throw std::runtime_error("Insufficient format");

    reader.nextTwoBytes(ret.compressionCode);
    reader.nextTwoBytes(ret.channels);
    reader.nextWord(ret.samplingRate);
    reader.nextWord(ret.bytesPerSecond);
    reader.nextTwoBytes(ret.blockAlign);
    reader.nextTwoBytes(ret.bitsPerSample);

    if (ret.compressionCode != 1) // 1 == PCM
        throw std::runtime_error("Not supported compression code");

    unsigned char dummy;
    for (unsigned int toSkip = size - 16; toSkip > 0; toSkip -= 1) {
        reader.nextByte(dummy);
    }

    return ret;
}
