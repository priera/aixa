#ifndef AIXA_SRC_MAINLIB_STREAM_WAVREADER_H
#define AIXA_SRC_MAINLIB_STREAM_WAVREADER_H

#include <string>
#include <fstream>
#include <vector>

#include "mainlib/stream/FileReader.h"
#include "mainlib/stream/wav/wavparts/WavFormat.h"

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
    std::vector<std::string> infoData;

    unsigned int processedBytes;
    unsigned int fileSize;
};


#endif //AIXA_SRC_MAINLIB_STREAM_WAVREADER_H
