#ifndef ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVREADER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVREADER_H

#include <string>
#include <fstream>

#include "FileReader.h"
#include "WavFormat.h"

class WavReader {
public:
    WavReader(const std::string &path);
    ~WavReader() =  default;

    void run();

private:
    bool probe(unsigned int & fileSize);

    void readFileHeader(unsigned int & fileSize);
    void readChuncks();
    void skipUnknownTag(unsigned int size);

    FileReader f;
    WavFormat format;

    unsigned int processedBytes;
    unsigned int fileSize;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVREADER_H
