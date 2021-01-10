#include "BasicByteReader.h"

#include <cassert>
#include <iostream>

BasicByteReader::BasicByteReader(std::ifstream in) : f(std::move(in)), lastByte(0), lastByteRemBits(0) {}

unsigned int BasicByteReader::nextWord() {
    f.read(&wt.bytes[0], 4);
    return wt.word;
}

unsigned short BasicByteReader::nextShort() {
    wt.word = 0;
    f.read(&wt.bytes[0], 2);
    return wt.word;
}

unsigned char BasicByteReader::nextByte() {
    if (lastByteRemBits == 0) {
        return privNextByte();
    } else {
        return nextNBits(8);
    }
}

unsigned short BasicByteReader::nextNBits(unsigned char n) {
    assert(n <= 16);

    unsigned char mask = (1 << lastByteRemBits) - 1;
    unsigned short ret = lastByte & mask;

    if (n <= lastByteRemBits) {
        // Requested bits are in the last byte read. No more data reads are needed
        unsigned char diff = lastByteRemBits - n;
        ret >>= diff;
        lastByteRemBits = diff;
        return ret;
    } else {
        // Last byte does only have a part of the requested bits.
        unsigned char toRead = n - lastByteRemBits;
        unsigned char b;
        unsigned char i;
        for (i = 0; i < toRead; i += 8) {
            b = privNextByte();
            unsigned char toExtract = (toRead - i > 8) ? 8 : toRead - i;
            ret = (ret << toExtract) + (b >> (8 - toExtract));
        }

        lastByte = b;
        lastByteRemBits = i - toRead;

        return ret;
    }
}

bool BasicByteReader::nextBit() { return nextNBits(1); }

void BasicByteReader::skipNBits(unsigned char n) { nextNBits(n); }

void BasicByteReader::skipBytes(long count) { f.seekg(f.tellg() + count); }

std::streamsize BasicByteReader::extractBytes(char *buff, size_t count) {
    f.read(buff, count);
    return f.gcount();
}

bool BasicByteReader::ended() const {
    bool fileOk = static_cast<bool>(f);
    return !fileOk || f.eof();
}