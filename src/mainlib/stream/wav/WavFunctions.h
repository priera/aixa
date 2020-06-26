#ifndef AIXA_SRC_MAINLIB_STREAM_WAV_WAVFUNCTIONS_H
#define AIXA_SRC_MAINLIB_STREAM_WAV_WAVFUNCTIONS_H

#include <cstddef>

#include <mainlib/stream/FileReader.h>
#include <mainlib/stream/wav/wavparts/WavFormat.h>

namespace WavFunctions {
    size_t readHeader(FileReader &reader, unsigned int &fileSize);
    size_t readFormat(FileReader &reader, unsigned int size, WavFormat &format);
};


#endif //AIXA_SRC_MAINLIB_STREAM_WAV_WAVFUNCTIONS_H
