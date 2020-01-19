#include "FileReader.h"

FileReader::FileReader(const std::string & path) {
    f.open(path.c_str(), std::ios_base::binary);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open stream");
    }
}

FileReader::~FileReader() {
    f.close();
}

bool FileReader::nextIdTag(std::string &str) {
    f.read(&wt.bytes[0], 4);
    str.assign(&wt.bytes[0], 4);

    return f.good();
}

bool FileReader::nextWord(unsigned int &w) {
    f.read(&wt.bytes[0], 4);
    w = wt.word;

    return f.good();
}

bool FileReader::nextByte(char &b) {
    return f.read(&b, 1).good();
}