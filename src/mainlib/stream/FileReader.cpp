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

void FileReader::seekToBeginning() {
    f.seekg(0);

    check();
}

void FileReader::nextIdTag(std::string &str) {
    nextIdTagNoCheck(str);

    check();
}

void FileReader::nextWord(unsigned int &w) {
    nextWordNoCheck(w);

    check();
}

void FileReader::nextIdTagNoCheck(std::string &str) {
    f.read(&wt.bytes[0], 4);
    str.assign(&wt.bytes[0], 4);

}

void FileReader::nextWordNoCheck(unsigned int &w) {
    f.read(&wt.bytes[0], 4);
    w = wt.word;
}

void FileReader::nextTwoBytes(unsigned int &hw) {
    wt.word = 0;
    f.read(&wt.bytes[0], 2);
    hw = wt.word;

    check();
}

void FileReader::nextByte(unsigned char &b) {
    f >> b;

    check();
}

void FileReader::readString(std::string &str, unsigned int size) {
    f.read(&str[0], size);

    check();
}

void FileReader::nextChunkInfo(std::string &idTag, unsigned int &word) {
    nextIdTagNoCheck(idTag);
    nextWordNoCheck(word);

    check();
}