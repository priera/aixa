#include "FileReader.h"

#include <iostream>

void FileReader::nextIdTag(std::string &str) { readString(str, 4); }

void FileReader::readString(std::string &str, unsigned int size) { extractBytes(&str[0], size); }

void FileReader::nextChunkInfo(std::string &idTag, unsigned int &chunkSize) {
    nextIdTag(idTag);
    nextWord(chunkSize);
}
