#include "FileReader.h"

FileReader::FileReader(const std::string &path) {
    f.open(path.c_str(), std::ios_base::binary);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open stream");
    }
}

FileReader::~FileReader() { f.close(); }

void FileReader::seekToBeginning() { f.seekg(0); }

void FileReader::nextIdTag(std::string &str) {
    f.read(&wt.bytes[0], 4);
    str.assign(&wt.bytes[0], 4);
}

void FileReader::nextWord(unsigned int &w) {
    f.read(&wt.bytes[0], 4);
    w = wt.word;
}

void FileReader::nextTwoBytes(unsigned int &hw) {
    wt.word = 0;
    f.read(&wt.bytes[0], 2);
    hw = wt.word;
}

unsigned char FileReader::nextByte() {
    unsigned char b;
    f >> b;
    return b;
}

void FileReader::readString(std::string &str, unsigned int size) { f.read(&str[0], size); }

void FileReader::nextChunkInfo(std::string &idTag, unsigned int &chunkSize) {
    nextIdTag(idTag);
    nextWord(chunkSize);
}

void FileReader::skipBytes(long count) { f.seekg(f.tellg() + count); }

std::streamsize FileReader::extractBytes(char *buff, size_t count) {
    f.read(buff, count);
    return f.gcount();
}

bool FileReader::ended() const {
    bool fileOk = static_cast<bool>(f);
    return !fileOk || f.eof();
}
