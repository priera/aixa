#include "WavFunctions.h"

#include <mainlib/stream/wav/wavparts/WavFormat.h>

namespace WavFunctions {

    size_t readHeader(FileReader &reader, unsigned int &fileSize) {
        std::string riffTag, waveTag;

        reader.nextIdTag(riffTag);
        reader.nextWord(fileSize);
        reader.nextIdTag(waveTag);

        if (riffTag != "RIFF" || waveTag != "WAVE") {
            throw std::runtime_error("incorrect header");
        }

        return 4; //2 bytes riffTag  + 2 bytes for waveTag
    }

    size_t readFormat(FileReader &reader, unsigned int size, WavFormat &format) {
        const unsigned int PCM_COMPRESSION_CODE = 1;

        if (size < 16)
            throw std::runtime_error("Insufficient format");

        reader.nextTwoBytes(format.compressionCode);
        reader.nextTwoBytes(format.channels);
        reader.nextWord(format.samplingRate);
        reader.nextWord(format.bytesPerSecond);
        reader.nextTwoBytes(format.blockAlign);
        reader.nextTwoBytes(format.bitsPerSample);

        if (format.compressionCode != PCM_COMPRESSION_CODE)
            throw std::runtime_error("Not supported compression code");

        unsigned char dummy;
        for (unsigned int toSkip = size - 16; toSkip > 0; toSkip -= 1) {
            reader.nextByte(dummy);
        }

        return size;
    }
}