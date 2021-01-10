#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H

#include <mainlib/stream/in/ByteReader.h>

#include "ByteReservoir.h"

class MainDataReader : public ByteReader {
public:
    explicit MainDataReader(ByteReader &inStream) :
        reservoir(),
        reservoirReader(nullptr),
        inStream(&inStream),
        inStreamRead(0),
        currentReader(nullptr) {}
    ~MainDataReader() override = default;

    unsigned int nextWord() override {
        checkReader();
        return currentReader->nextWord();
    }

    unsigned short nextShort() override {
        checkReader();
        return currentReader->nextShort();
    }

    unsigned char nextByte() override {
        checkReader();
        return currentReader->nextByte();
    }

    unsigned short nextNBits(unsigned char n) override {
        checkReader();
        return currentReader->nextNBits(n);
    }

    bool nextBit() override {
        checkReader();
        return currentReader->nextBit();
    }

    void skipNBits(unsigned char n) override {
        checkReader();
        return currentReader->skipNBits(n);
    }

    bool ended() const override { return inStream->ended(); }

    long tellg() const override { return inStream->tellg() - inStreamRead; }

    void skipBytes(long count) override {
        checkReader();
        return currentReader->skipBytes(count);
    }

    std::streamsize extractBytes(char *buff, std::size_t count) override {
        checkReader();
        return currentReader->extractBytes(buff, count);
    }

    void startFrame(unsigned int mainDataBegin);
    void frameEnded(unsigned int remainingBits);
    void advanceReservoir(unsigned int nBytes);

private:
    void checkReader() {
        if (currentReader == reservoirReader.get() && reservoir.consumed()) {
            currentReader = inStream;
            reservoirReader = nullptr;
        }
    }

    ByteReservoir reservoir;
    std::unique_ptr<ByteReader> reservoirReader;
    ByteReader *inStream;

    long inStreamRead;
    ByteReader *currentReader;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
