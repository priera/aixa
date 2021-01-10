#ifndef AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H

#include <fstream>
#include <iostream>

#include "ByteReader.h"

class BasicByteReader : public ByteReader {
public:
    ~BasicByteReader() override = default;

    unsigned int nextWord() override;
    unsigned short nextShort() override;
    unsigned char nextByte() override;
    unsigned short nextNBits(unsigned char n) override;
    bool nextBit() override;
    void skipNBits(unsigned char n) override;
    void skipBytes(long count) override;
    std::streamsize extractBytes(char *buff, std::size_t count) override;
    bool ended() const override;

protected:
    explicit BasicByteReader(std::ifstream in);

    std::ifstream f;

private:
    union WordType {
        unsigned int word;
        char bytes[4];
    };

    unsigned char privNextByte() {
        char b;
        f.read(&b, 1);
        return b;
    }

    WordType wt;

    unsigned char lastByte;
    unsigned char lastByteRemBits;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
