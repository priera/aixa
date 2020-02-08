#include "WavReader.h"

WavReader::WavReader(const std::string & path) : f(path) {
    bool ok = probe(fileSize);
    processedBytes = 0;
}

bool WavReader::probe(unsigned int & fileSize) {
    bool ret = true;
    try {
        readFileHeader(fileSize);
        f.seekToBeginning();
    } catch (std::runtime_error &e) {
        ret = false;
    }

    return ret;
}

void WavReader::run() {
    unsigned int fileSize;
    readFileHeader(fileSize);

    readChuncks();
}

void WavReader::readFileHeader(unsigned int & fileSize) {
    std::string riffTag, waveTag;

    f.nextIdTag(riffTag);

    f.nextWord(fileSize);

    f.nextIdTag(waveTag);
    processedBytes += 4;

    if (riffTag != "RIFF" || waveTag != "WAVE") {
        throw std::runtime_error("incorrect header");
    }
}

void WavReader::readChuncks() {
    std::string tag;
    unsigned int size;
    f.nextChunkInfo(tag, size);
    processedBytes += 8;

    if (tag == "fmt ") {

    } else if (tag == "LIST") {

    } else if (tag == "data") {

    } else {
        skipUnknownTag(size);
    }

    processedBytes += size;
}

void WavReader::skipUnknownTag(unsigned int size) {
    unsigned int dummy;
    for (; size > 0; size -= 4) {
        f.nextWord(dummy);
    }
}
