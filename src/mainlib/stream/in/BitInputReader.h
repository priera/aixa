#ifndef AIXA_SRC_MAINLIB_STREAM_IN_BITINPUTREADER_H
#define AIXA_SRC_MAINLIB_STREAM_IN_BITINPUTREADER_H

#include <cstddef>
#include <ios>

class BitInputReader {
public:
    virtual ~BitInputReader() = default;

    virtual unsigned int nextWord(unsigned int& result) = 0;
    virtual unsigned int nextShort(short& result) = 0;
    virtual unsigned int nextByte(unsigned char& result) = 0;
    virtual bool nextBit() = 0;
    virtual unsigned int nextNBits(unsigned char n, unsigned short& result) = 0;

    virtual void skipNBits(unsigned char n) = 0;
    virtual void skipBytes(long count) = 0;
    virtual std::streamsize extractBytes(char* buff, std::size_t count) = 0;

    virtual void byteAlign() = 0;

    virtual bool ended() const = 0;
    virtual std::size_t bitsRead() const = 0;
};
#endif  // AIXA_SRC_MAINLIB_STREAM_IN_BITINPUTREADER_H
