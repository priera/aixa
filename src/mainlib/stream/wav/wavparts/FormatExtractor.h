#ifndef AIXA_SRC_MAINLIB_STREAM_FORMATEXTRACTOR_H
#define AIXA_SRC_MAINLIB_STREAM_FORMATEXTRACTOR_H


#include "WavFormat.h"
#include "mainlib/stream/FileReader.h"

class FormatExtractor {
public:
    FormatExtractor() = default;
    virtual ~FormatExtractor() = default;

    static WavFormat readFormat(FileReader &reader, unsigned int size);
};


#endif //AIXA_SRC_MAINLIB_STREAM_FORMATEXTRACTOR_H
