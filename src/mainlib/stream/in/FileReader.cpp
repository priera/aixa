#include "FileReader.h"

#include <iostream>

FileReader *FileReader::buildForFile(const std::string &path) {
    std::ifstream f;
    f.open(path.c_str(), std::ios_base::binary);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open stream");
    }

    return new FileReader(std::move(f));
}

void FileReader::seekToBeginning() { f.seekg(0); }

void FileReader::nextIdTag(std::string &str) { readString(str, 4); }

void FileReader::readString(std::string &str, unsigned int size) { f.read(&str[0], size); }

void FileReader::nextChunkInfo(std::string &idTag, unsigned int &chunkSize) {
    nextIdTag(idTag);
    chunkSize = nextWord();
}
