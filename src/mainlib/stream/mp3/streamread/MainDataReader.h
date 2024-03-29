#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H

#include <mainlib/stream/in/sizes.h>

#include <iostream>

#include "ByteReservoir.h"

class MainDataReader : public AbstractBitInputReader {
public:
    explicit MainDataReader(std::unique_ptr<BitInputReader> inStream) :
        reservoir(), reservoirReader(nullptr), inStream(std::move(inStream)), inStreamAtStartFrame(0) {
        currentReader = this->inStream.get();
    }

    ~MainDataReader() override = default;

    unsigned int tryExtractWord(unsigned int& result) override {
        unsigned short tmp, tmp2;
        dualRead(tmp, S_SHORT);
        dualRead(tmp2, S_SHORT);
        result = (tmp << S_SHORT) + tmp2;
        return S_WORD;
    }

    unsigned int tryExtractShort(short& result) override {
        unsigned short tmp;
        auto ret = dualRead(tmp, S_SHORT);
        result = static_cast<short>(tmp);
        return ret;
    }

    unsigned int tryExtractByte(unsigned char& result) override {
        unsigned short tmp;
        auto ret = dualRead(tmp, S_BYTE);
        result = static_cast<unsigned char>(tmp);
        return ret;
    }

    unsigned int tryExtractNBits(unsigned char n, unsigned short& result) override {
        return dualRead(result, n);
    }

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

    std::size_t bitsRead() const override { return frameBitsRead; }

    void skipBytes(long count) override {
        frameBitsRead += count * S_BYTE;
        return inStream->skipBytes(count);
    }

    std::streamsize extractBytes(char* buff, std::size_t count) override {
        frameBitsRead += count * S_BYTE;
        return inStream->extractBytes(buff, count);
    }

    void seekToBeginning() override {
        reservoir.clear();
        inStream->seekToBeginning();
        currentReader = inStream.get();
    }

    unsigned int inStreamPos() const { return inStream->bitsRead(); }
    void startFrame(unsigned int mainDataBegin);
    void frameEnded(unsigned int frameSize, unsigned int bytesInHeaders);

private:
    unsigned int dualRead(unsigned short& result, unsigned int toRead) {
        frameBitsRead += toRead;
        checkReader();
        unsigned short tmp;
        auto read = currentReader->tryExtractNBits(toRead, tmp);
        if (read < toRead) {
            checkReader();
            unsigned short remainingBits = toRead - read;
            unsigned short remainder;
            currentReader->tryExtractNBits(remainingBits, remainder);
            tmp <<= remainingBits;
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
    std::size_t frameBitsRead;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
