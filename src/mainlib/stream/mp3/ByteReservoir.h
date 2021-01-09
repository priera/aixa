#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H

#include <mainlib/stream/ByteReader.h>

#include <vector>

class ByteReservoir {
public:
    explicit ByteReservoir();
    virtual ~ByteReservoir() noexcept = default;

    void append(unsigned int remainingBytes, ByteReader& reader);
    void advanceReservoir(unsigned int nBytes);
    std::vector<char> extract(unsigned int nBytes);

private:
    static constexpr unsigned int RESERVOIR_SIZE = 512;

    std::vector<char> reservoir;
    std::vector<char> readBuffer;
    unsigned int currentPos;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H
