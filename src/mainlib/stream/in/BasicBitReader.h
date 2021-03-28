#ifndef AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H

#include <cassert>
#include <fstream>

#include "BitInputReader.h"
#include "sizes.h"

template <class ReadOperations>
class BasicBitReader : public BitInputReader {
public:
    explicit BasicBitReader(ReadOperations ops) : ops(std::move(ops)), lastByteRemBits(0), totalBits(0) {}
    ~BasicBitReader() override = default;

    unsigned int nextWord() override {
        ops.readNBytes(&wt.bytes[0], S_WORD / S_BYTE);
        totalBits += S_WORD;
        return wt.word;
    }

    unsigned short nextShort() override {
        wt.word = 0;
        ops.readNBytes(&wt.bytes[0], S_SHORT / S_BYTE);
        totalBits += S_SHORT;
        return wt.word;
    }

    unsigned char nextByte() override {
        totalBits += S_BYTE;
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
        }

        totalBits += n;
        return ret;
    }

    bool nextBit() override { return nextNBits(1); }

    void skipNBits(unsigned char n) override { nextNBits(n); }

    void byteAlign() override {
        totalBits += lastByteRemBits;
        lastByteRemBits = 0;
    }

    void skipBytes(long count) override {
        ops.skipNBytes(count);
        totalBits += count * S_BYTE;
    }

    std::streamsize extractBytes(char *buff, std::size_t count) override {
        auto ret = ops.readNBytes(buff, count);
        totalBits += count * S_BYTE;
        return ret;
    }

    unsigned long bitsRead() const override { return totalBits; }

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
        return b;
    }

    WordType wt;

    unsigned char lastByte;
    unsigned char lastByteRemBits;

    unsigned long totalBits;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
