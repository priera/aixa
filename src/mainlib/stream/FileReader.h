#ifndef ALSAPLAYGROUND_SRC_MAINLIB_STREAM_FILEREADER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_STREAM_FILEREADER_H


#include <string>
#include <fstream>

class FileReader {
public:
    FileReader(const std::string & path);
    virtual ~FileReader();

    bool nextIdTag(std::string &str);
    bool nextWord(unsigned int &w);
    bool nextByte(char &b);


private:
    union WordType {
        unsigned int word;
        char bytes[4];
    };

    std::ifstream f;
    WordType wt;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_STREAM_FILEREADER_H
