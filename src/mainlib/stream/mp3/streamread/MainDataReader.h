#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H

#include <mainlib/stream/in/BitInputReader.h>
#include <mainlib/stream/in/sizes.h>

#include <iostream>

#include "ByteReservoir.h"

class MainDataReader : public BitInputReader {
public:
    explicit MainDataReader(std::unique_ptr<BitInputReader> inStream) :
        reservoir(), reservoirReader(nullptr), inStream(std::move(inStream)), inStreamAtStartFrame(0) {
        currentReader = this->inStream.get();
    }

    ~MainDataReader() override = default;

    unsigned int nextWord(unsigned int& result) override {
        unsigned short tmp, tmp2;
        dualRead(tmp, S_SHORT);
        dualRead(tmp2, S_SHORT);
        result = (tmp << S_SHORT) + tmp2;
        return S_WORD;
    }

    unsigned int nextShort(short& result) override {
        unsigned short tmp;
        auto ret = dualRead(tmp, S_SHORT);
        result = static_cast<short>(tmp);
        return ret;
    }

    unsigned int nextByte(unsigned char& result) override {
        unsigned short tmp;
        auto ret = dualRead(tmp, S_BYTE);
        result = static_cast<unsigned char>(tmp);
        return ret;
    }

    unsigned int nextNBits(unsigned char n, unsigned short& result) override { return dualRead(result, n); }

    bool nextBit() override {
        frameBitsRead++;
        checkReader();
        return currentReader->nextBit();
    }

    void byteAlign() override { inStream->byteAlign(); }

    void skipNBits(unsigned char n) override {
        frameBitsRead += n;
        inStream->skipNBits(n);
    }

    bool ended() const override { return inStream->ended(); }

    unsigned long bitsRead() const override { return frameBitsRead; }

    void skipBytes(long count) override {
        frameBitsRead += count * S_BYTE;
        return inStream->skipBytes(count);
    }

    std::streamsize extractBytes(char* buff, std::size_t count) override {
        frameBitsRead += count * S_BYTE;
        return inStream->extractBytes(buff, count);
    }

    unsigned int inStreamPos() const { return inStream->bitsRead(); }
    void startFrame(unsigned int mainDataBegin);
    void frameEnded(unsigned int frameSize, unsigned int bytesInHeaders);

private:
    unsigned int dualRead(unsigned short& result, unsigned int toRead) {
        frameBitsRead += toRead;
        checkReader();
        unsigned short tmp;
        auto read = currentReader->nextNBits(toRead, tmp);
        if (read < toRead) {
            checkReader();
            unsigned short remainder;
            currentReader->nextNBits(toRead - read, remainder);
            tmp <<= read;
            tmp += remainder;
        }
        result = tmp;
        return S_WORD;
    }

    void checkReader() {
        if (currentReader == reservoirReader.get() && currentReader->ended()) {
            currentReader = inStream.get();
            reservoirReader = nullptr;
        }
    }

    ByteReservoir reservoir;
    std::unique_ptr<BitInputReader> reservoirReader;
    std::unique_ptr<BitInputReader> inStream;

    unsigned long inStreamAtStartFrame;
    BitInputReader* currentReader;
    unsigned long frameBitsRead;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
