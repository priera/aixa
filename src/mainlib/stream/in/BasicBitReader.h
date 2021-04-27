#ifndef AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
#define AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H

#include <cassert>
#include <fstream>

#include "AbstractBitInputReader.h"
#include "sizes.h"

template <class ReadOperations>
class BasicBitReader : public AbstractBitInputReader {
public:
    explicit BasicBitReader(ReadOperations ops) : ops(std::move(ops)), lastByteRemBits(0), totalBits(0) {}
    ~BasicBitReader() override = default;

    unsigned int tryExtractWord(unsigned int& result) override {
        ops.readNBytes(&wt.bytes[0], S_WORD / S_BYTE);
        totalBits += S_WORD;
        result = wt.word;
        return S_WORD;
    }

    unsigned int tryExtractShort(short& result) override {
        wt.word = 0;
        ops.readNBytes(&wt.bytes[0], S_SHORT / S_BYTE);
        totalBits += S_SHORT;
        result = wt.word;
        return S_SHORT;
    }

    unsigned int tryExtractByte(unsigned char& result) override {
        totalBits += S_BYTE;
        if (lastByteRemBits == 0) {
            return privNextByte(result);
        } else {
            unsigned short resultShort;
            auto read = tryExtractNBits(8, resultShort);
            result = resultShort;
            return read;
        }
    }

    unsigned int tryExtractNBits(unsigned char n, unsigned short& result) override {
        assert(n <= S_SHORT);

        unsigned char mask = (1 << lastByteRemBits) - 1;
        unsigned short ret = lastByte & mask;
        unsigned int actuallyRead;

        if (n <= lastByteRemBits) {
            // Requested bits are in the last byte read. No more data reads are needed
            unsigned char diff = lastByteRemBits - n;
            ret >>= diff;
            lastByteRemBits = diff;
            totalBits += n;
            actuallyRead = n;
        } else {
            // Last byte does only have a part of the requested bits.
            unsigned char toRead = n - lastByteRemBits;
            unsigned char b;
            unsigned char i;
            for (i = 0; i < toRead; i += S_BYTE) {
                if (privNextByte(b) == 0) {
                    break;
                }

                unsigned char toExtract = (toRead - i > S_BYTE) ? S_BYTE : toRead - i;
                ret <<= toExtract;
                ret += (b >> (S_BYTE - toExtract));
            }

            if (i >= toRead) {
                actuallyRead = n;
                lastByte = b;
                lastByteRemBits = i - toRead;
            } else {
                actuallyRead = lastByteRemBits + i;
                lastByteRemBits = 0;
            }
            totalBits += actuallyRead;
        }

        result = ret;
        return actuallyRead;
    }

    bool nextBit() override {
        unsigned short result;
        tryExtractNBits(1, result);
        return result;
    }

    void skipNBits(unsigned char n) override {
        unsigned short dummy;
        tryExtractNBits(n, dummy);
    }

    void byteAlign() override {
        totalBits += lastByteRemBits;
        lastByteRemBits = 0;
    }

    void skipBytes(long count) override {
        ops.skipNBytes(count);
        totalBits += count * S_BYTE;
    }

    std::streamsize extractBytes(char* buff, std::size_t count) override {
        auto ret = ops.readNBytes(buff, count);
        totalBits += count * S_BYTE;
        return ret;
    }

    unsigned long bitsRead() const override { return totalBits; }

    bool ended() const override { return ops.ended() && lastByteRemBits == 0; }

    void seekToBeginning() override {
        ops.seekToBeginning();
        totalBits = 0;
        lastByteRemBits = 0;
    }

private:
    ReadOperations ops;

    union WordType {
        unsigned int word;
        char bytes[4];
    };

    std::streamsize privNextByte(unsigned char& ret) { return ops.readNBytes(&ret, 1); }

    WordType wt;

    unsigned char lastByte;
    unsigned char lastByteRemBits;

    unsigned long totalBits;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_BYTEREADER_H
