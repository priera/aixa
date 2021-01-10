#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H

#include <mainlib/stream/in/ByteReader.h>

#include "ByteReservoir.h"

class MainDataReader : public ByteReader {
public:
    MainDataReader(ByteReservoir &reservoir, ByteReader &inStream) :
        reservoir(&reservoir),
        inStream(&inStream),
        currentReader(nullptr) {}
    ~MainDataReader() override = default;

private:
    ByteReservoir *reservoir;
    ByteReader *inStream;
    ByteReader *currentReader;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
