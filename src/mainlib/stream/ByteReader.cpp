#include "ByteReader.h"

#include <cassert>
#include <iostream>

ByteReader::ByteReader(const std::string &path) : lastByte(0), lastByteRemBits(0) {
    f.open(path.c_str(), std::ios_base::binary);
    if (!f.is_open()) {
        throw std::runtime_error("Failed to open stream");
    }
}

ByteReader::~ByteReader() { f.close(); }

unsigned int ByteReader::nextWord() {
    f.read(&wt.bytes[0], 4);
    return wt.word;
}

unsigned short ByteReader::nextShort() {
    wt.word = 0;
    f.read(&wt.bytes[0], 2);
    return wt.word;
}

unsigned char ByteReader::nextByte() {
    if (lastByteRemBits == 0) {
        return privNextByte();
    } else {
        return nextNBits(8);
    }
}

unsigned short ByteReader::nextNBits(unsigned char n) {
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

bool ByteReader::nextBit() { return nextNBits(1); }

void ByteReader::skipNBits(unsigned char n) { nextNBits(n); }

void ByteReader::skipBytes(long count) { f.seekg(f.tellg() + count); }

std::streamsize ByteReader::extractBytes(char *buff, size_t count) {
    f.read(buff, count);
    return f.gcount();
}
