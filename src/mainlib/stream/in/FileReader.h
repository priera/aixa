#ifndef AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_FILEREADER_H

#include <string>

#include "BasicByteReader.h"
#include "InStreamOperations.h"

class FileReader : public BasicByteReader<InStreamOperations> {
public:
    explicit FileReader(const std::string &path) : BasicByteReader(InStreamOperations(path)) {}

    ~FileReader() override = default;

    void nextChunkInfo(std::string &idTag, unsigned int &chunkSize);
    void nextIdTag(std::string &str);
    void readString(std::string &str, unsigned int size);
};

#endif  // AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
