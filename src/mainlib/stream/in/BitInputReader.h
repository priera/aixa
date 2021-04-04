#ifndef AIXA_SRC_MAINLIB_STREAM_IN_BITINPUTREADER_H
#define AIXA_SRC_MAINLIB_STREAM_IN_BITINPUTREADER_H

#include <cstddef>
#include <ios>

class BitInputReader {
public:
    virtual ~BitInputReader() = default;

    virtual unsigned int tryExtractWord(unsigned int& result) = 0;
    virtual unsigned int tryExtractShort(short& result) = 0;
    virtual unsigned int tryExtractByte(unsigned char& result) = 0;
    virtual bool nextBit() = 0;
    virtual unsigned int tryExtractNBits(unsigned char n, unsigned short& result) = 0;

    virtual unsigned int nextWord() = 0;
    virtual short nextShort() = 0;
    virtual unsigned char nextByte() = 0;
    virtual unsigned short nextNBits(unsigned char n) = 0;

    virtual void skipNBits(unsigned char n) = 0;
    virtual void skipBytes(long count) = 0;
    virtual std::streamsize extractBytes(char* buff, std::size_t count) = 0;

    virtual void byteAlign() = 0;

    virtual bool ended() const = 0;
    virtual std::size_t bitsRead() const = 0;
};
#endif  // AIXA_SRC_MAINLIB_STREAM_IN_BITINPUTREADER_H
