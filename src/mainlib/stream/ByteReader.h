#ifndef AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H

#include <fstream>
#include <iostream>

class ByteReader {
public:
    explicit ByteReader(const std::string &path);
    virtual ~ByteReader();

    unsigned int nextWord();
    unsigned short nextShort();
    unsigned char nextByte();
    unsigned short nextNBits(unsigned char n);
    bool nextBit();
    void skipNBits(unsigned char n);

    void skipBytes(long count);
    std::streamsize extractBytes(char *buff, std::size_t count);

protected:
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
