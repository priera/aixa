#ifndef AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_FILEREADER_H

#include <fstream>
#include <string>

class FileReader {
public:
    explicit FileReader(const std::string &path);
    virtual ~FileReader();

    void seekToBeginning();

    void nextChunkInfo(std::string &idTag, unsigned int &chunkSize);
    void nextIdTag(std::string &str);
    void nextWord(unsigned int &w);
    void nextTwoBytes(unsigned int &hw);
    unsigned char nextByte();
    unsigned short nextNBits(unsigned char n, unsigned char ahead, unsigned char aheadCount,
                             unsigned char &remainder, unsigned char &remainderCount);
    void readString(std::string &str, unsigned int size);
    void skipBytes(long count);
    std::streamsize extractBytes(char *buff, size_t count);
    bool ended() const;

private:
    union WordType {
        unsigned int word;
        char bytes[4];
    };

    std::ifstream f;
    WordType wt;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
