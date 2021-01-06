#include "FileReader.h"

#include <iostream>

void FileReader::seekToBeginning() { f.seekg(0); }

void FileReader::nextIdTag(std::string &str) { readString(str, 4); }

void FileReader::readString(std::string &str, unsigned int size) { f.read(&str[0], size); }

void FileReader::nextChunkInfo(std::string &idTag, unsigned int &chunkSize) {
    nextIdTag(idTag);
    chunkSize = nextWord();
}

bool FileReader::ended() const {
    bool fileOk = static_cast<bool>(f);
    return !fileOk || f.eof();
}
