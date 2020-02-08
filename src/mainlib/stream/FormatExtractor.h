#ifndef ALSAPLAYGROUND_SRC_MAINLIB_STREAM_FORMATEXTRACTOR_H
#define ALSAPLAYGROUND_SRC_MAINLIB_STREAM_FORMATEXTRACTOR_H


#include "WavFormat.h"
#include "FileReader.h"

class FormatExtractor {
public:
    FormatExtractor() = default;
    virtual ~FormatExtractor() = default;

    static WavFormat readFormat(FileReader &reader, unsigned int size);
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_STREAM_FORMATEXTRACTOR_H
