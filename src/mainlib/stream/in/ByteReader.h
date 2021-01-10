#ifndef AIXA_SRC_MAINLIB_STREAM_IN_BYTEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_IN_BYTEREADER_H

#include <cstddef>
#include <ios>

class ByteReader {
public:
    virtual ~ByteReader() = default;

    virtual unsigned int nextWord() = 0;
    virtual unsigned short nextShort() = 0;
    virtual unsigned char nextByte() = 0;
    virtual unsigned short nextNBits(unsigned char n) = 0;
    virtual bool nextBit() = 0;
    virtual void skipNBits(unsigned char n) = 0;
    virtual bool ended() const = 0;
    virtual void skipBytes(long count) = 0;
    virtual std::streamsize extractBytes(char *buff, std::size_t count) = 0;
};
#endif  // AIXA_SRC_MAINLIB_STREAM_IN_BYTEREADER_H
