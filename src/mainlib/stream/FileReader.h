#ifndef AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_FILEREADER_H


#include <string>
#include <fstream>

class FileReader {
public:
    explicit FileReader(const std::string &path);
    virtual ~FileReader();

    void seekToBeginning();

    void nextChunkInfo(std::string &idTag, unsigned int &chunkSize);
    void nextIdTag(std::string &str);
    void nextWord(unsigned int &w);
    void nextTwoBytes(unsigned int &hw);
    void nextByte(unsigned char &b);
    void readString(std::string &str, unsigned int size);

private:
    union WordType {
        unsigned int word;
        char bytes[4];
    };

    std::ifstream f;
    WordType wt;
};


#endif //AIXA_SRC_MAINLIB_STREAM_FILEREADER_H
