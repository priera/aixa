#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H

#include <mainlib/stream/in/BitInputReader.h>
#include <mainlib/stream/in/sizes.h>

#include "ByteReservoir.h"

class MainDataReader : public BitInputReader {
public:
    explicit MainDataReader(std::unique_ptr<BitInputReader> inStream) :
        reservoir(),
        reservoirReader(nullptr),
        inStream(std::move(inStream)),
        inStreamRead(0),
        currentReader(nullptr) {}
    ~MainDataReader() override = default;

    unsigned int nextWord() override {
        myBitsRead += S_WORD;
        checkReader();
        return currentReader->nextWord();
    }

    unsigned short nextShort() override {
        myBitsRead += S_SHORT;
        checkReader();
        return currentReader->nextShort();
    }

    unsigned char nextByte() override {
        myBitsRead += S_BYTE;
        checkReader();
        return currentReader->nextByte();
    }

    unsigned short nextNBits(unsigned char n) override {
        myBitsRead += n;
        checkReader();
        return currentReader->nextNBits(n);
    }

    bool nextBit() override {
        myBitsRead++;
        checkReader();
        return currentReader->nextBit();
    }

    void skipNBits(unsigned char n) override {
        myBitsRead += n;
        checkReader();
        return currentReader->skipNBits(n);
    }

    bool ended() const override { return inStream->ended(); }

    unsigned long bitsRead() const override { return myBitsRead; }

    void skipBytes(long count) override {
        myBitsRead += count * S_BYTE;
        checkReader();
        return currentReader->skipBytes(count);
    }

    std::streamsize extractBytes(char *buff, std::size_t count) override {
        myBitsRead += count * S_BYTE;
        checkReader();
        return currentReader->extractBytes(buff, count);
    }

    unsigned long streamConsumedBytes() const { return (inStream->bitsRead() / S_BYTE) - inStreamRead; }

    void startFrame(unsigned int mainDataBegin);
    void frameEnded(unsigned int remainingBits);

private:
    void checkReader() {
        if (currentReader == reservoirReader.get() && reservoir.consumed()) {
            currentReader = inStream.get();
            reservoirReader = nullptr;
        }
    }

    ByteReservoir reservoir;
    std::unique_ptr<BitInputReader> reservoirReader;
    std::unique_ptr<BitInputReader> inStream;

    unsigned long inStreamRead;
    BitInputReader *currentReader;
    unsigned long myBitsRead;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MAINDATAREADER_H
