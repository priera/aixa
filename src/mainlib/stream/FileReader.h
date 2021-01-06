#ifndef AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_FILEREADER_H

#include <string>

#include "ByteReader.h"

class FileReader : public ByteReader {
public:
    explicit FileReader(const std::string &path) : ByteReader(path) {}
    ~FileReader() override = default;

    void seekToBeginning();

    void nextChunkInfo(std::string &idTag, unsigned int &chunkSize);
    void nextIdTag(std::string &str);
    void readString(std::string &str, unsigned int size);
    bool ended() const;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
