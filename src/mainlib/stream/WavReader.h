#ifndef ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVREADER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVREADER_H

#include <string>
#include <fstream>

#include "FileReader.h"

class WavReader {
public:
    WavReader(const std::string &path);
    ~WavReader() =  default;

    void run();

private:
    FileReader f;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVREADER_H
