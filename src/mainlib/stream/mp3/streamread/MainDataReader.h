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

    unsigned int nextWord() override {
        frameBitsRead += S_WORD;
        checkReader();
        return currentReader->nextWord();
    }

    unsigned short nextShort() override {
        frameBitsRead += S_SHORT;
        checkReader();
        return currentReader->nextShort();
    }

    unsigned char nextByte() override {
        frameBitsRead += S_BYTE;
        checkReader();
        return currentReader->nextByte();
    }

    unsigned short nextNBits(unsigned char n) override {
        frameBitsRead += n;
        checkReader();
        return currentReader->nextNBits(n);
    }

    bool nextBit() override {
        frameBitsRead++;
        checkReader();
        return currentReader->nextBit();
    }

    void byteAlign() override { inStream->byteAlign(); }

    void skipNBits(unsigned char n) override {
        frameBitsRead += n;
        checkReader();
        return currentReader->skipNBits(n);
    }

    bool ended() const override { return inStream->ended(); }

    unsigned long bitsRead() const override { return frameBitsRead; }

    void skipBytes(long count) override {
        frameBitsRead += count * S_BYTE;
        checkReader();
        return currentReader->skipBytes(count);
    }

    std::streamsize extractBytes(char *buff, std::size_t count) override {
        frameBitsRead += count * S_BYTE;
        checkReader();
        return currentReader->extractBytes(buff, count);
    }

    unsigned int inStreamPos() const { return inStream->bitsRead(); }
    void startFrame(unsigned int mainDataBegin);
    void frameEnded(unsigned int frameSize, unsigned int bytesInHeaders);

private:
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
    BitInputReader *currentReader;
    unsigned long frameBitsRead;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
