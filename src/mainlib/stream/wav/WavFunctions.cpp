#include "WavFunctions.h"

#include <mainlib/stream/wav/WavFormat.h>

namespace WavFunctions {

size_t readHeader(FileReader &reader, unsigned int &fileSize) {
    std::string riffTag, waveTag;

    reader.nextIdTag(riffTag);
    fileSize = reader.nextWord();
    reader.nextIdTag(waveTag);

    if (riffTag != "RIFF" || waveTag != "WAVE") {
        throw std::runtime_error("incorrect header");
    }

    return 4;  // 2 bytes riffTag  + 2 bytes for waveTag
}

size_t readFormat(FileReader &reader, unsigned int size, WavFormat &format) {
    const unsigned int PCM_COMPRESSION_CODE = 1;

    if (size < 16)
        throw std::runtime_error("Insufficient format");

    format.compressionCode = reader.nextShort();
    format.channels = reader.nextShort();
    format.samplingRate = reader.nextWord();
    format.bytesPerSecond = reader.nextWord();
    format.blockAlign = reader.nextShort();
    format.bitsPerSample = reader.nextShort();

    if (format.compressionCode != PCM_COMPRESSION_CODE)
        throw std::runtime_error("Not supported compression code");

    for (unsigned int toSkip = size - 16; toSkip > 0; toSkip -= 1) {
        reader.skipBytes(1);
    }

    return size;
}

std::vector<std::string> extractListData(FileReader &f, unsigned int size) {
    std::vector<std::string> ret;

    std::string infoTag;
    f.nextIdTag(infoTag);
    size -= 4;

    while (size > 0) {
        std::string infoIdTag;
        f.nextIdTag(infoIdTag);

        unsigned int textSize = f.nextWord();

        std::string infoText(textSize, '\0');
        f.readString(infoText, textSize);

        size -= 8 + textSize;
        ret.push_back(infoText);
    }

    return ret;
}

size_t skipUntilDataChunk(FileReader &reader) {
    size_t ret = 0;

    std::string tag;
    unsigned int size;
    reader.nextChunkInfo(tag, size);
    ret += 8;

    while (tag != "data") {
        reader.skipBytes(size);
        ret += size;

        reader.nextChunkInfo(tag, size);
        ret += 8;
    }

    return ret;
}
}  // namespace WavFunctions