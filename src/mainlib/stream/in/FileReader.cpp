#include "FileReader.h"

#include <iostream>

void FileReader::nextIdTag(std::string &str) { readString(str, 4); }

void FileReader::readString(std::string &str, unsigned int size) {
    auto tmp = std::string(size, '\0');
    extractBytes(&tmp[0], size);
    str = tmp;
}

void FileReader::nextChunkInfo(std::string &idTag, unsigned int &chunkSize) {
    nextIdTag(idTag);
    chunkSize = nextWord();
}
