#ifndef AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H

#include <cassert>
#include <fstream>
#include <iostream>

#include "ByteReader.h"

template <class ReadOperations>
class BasicByteReader : public ByteReader {
public:
    explicit BasicByteReader(ReadOperations ops) : ops(std::move(ops)), lastByteRemBits(0), bytesRead(0) {}
    ~BasicByteReader() override = default;

    unsigned int nextWord() override {
        ops.readNBytes(&wt.bytes[0], 4);
        bytesRead += 4;
        return wt.word;
    }

    unsigned short nextShort() override {
        wt.word = 0;
        ops.readNBytes(&wt.bytes[0], 2);
        bytesRead += 2;
        return wt.word;
    }

    unsigned char nextByte() override {
        if (lastByteRemBits == 0) {
            return privNextByte();
        } else {
            return nextNBits(8);
        }
    }

    unsigned short nextNBits(unsigned char n) override {
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

    bool nextBit() override { return nextNBits(1); }

    void skipNBits(unsigned char n) override { nextNBits(n); }

    void skipBytes(long count) override { ops.skipNBytes(count); }

    std::streamsize extractBytes(char *buff, std::size_t count) override {
        return ops.readNBytes(buff, count);
    }

    long tellg() const override { return bytesRead; }

    bool ended() const override { return ops.ended(); }

private:
    ReadOperations ops;

    union WordType {
        unsigned int word;
        char bytes[4];
    };

    unsigned char privNextByte() {
        char b;
        ops.readNBytes(&b, 1);
        bytesRead++;
        return b;
    }

    WordType wt;

    unsigned char lastByte;
    unsigned char lastByteRemBits;

    long bytesRead;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
