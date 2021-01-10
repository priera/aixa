#ifndef AIXA_SRC_MAINLIB_STREAM_WAV_WAVFUNCTIONS_H
#define AIXA_SRC_MAINLIB_STREAM_WAV_WAVFUNCTIONS_H

#include <mainlib/stream/in/FileReader.h>
#include <mainlib/stream/wav/WavFormat.h>

#include <cstddef>
#include <string>
#include <vector>

namespace WavFunctions {
size_t readHeader(FileReader &reader, unsigned int &fileSize);
size_t readFormat(FileReader &reader, unsigned int size, WavFormat &format);
std::vector<std::string> extractListData(FileReader &f, unsigned int size);
size_t skipUntilDataChunk(FileReader &reader);
};  // namespace WavFunctions

#endif  // AIXA_SRC_MAINLIB_STREAM_WAV_WAVFUNCTIONS_H
