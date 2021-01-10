#ifndef AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_FILEREADER_H

#include <string>

#include "BasicByteReader.h"

class FileReader : public BasicByteReader {
public:
    static FileReader *buildForFile(const std::string &path);

    ~FileReader() override = default;

    void seekToBeginning();

    void nextChunkInfo(std::string &idTag, unsigned int &chunkSize);
    void nextIdTag(std::string &str);
    void readString(std::string &str, unsigned int size);

protected:
    explicit FileReader(std::ifstream in) : BasicByteReader(std::move(in)) {}
};

#endif  // AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
