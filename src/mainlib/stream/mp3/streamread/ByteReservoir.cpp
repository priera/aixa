#include "ByteReservoir.h"

ByteReservoir::ByteReservoir() : reservoir(RESERVOIR_SIZE, 0), readBuffer(RESERVOIR_SIZE), currentPos(0) {}

void ByteReservoir::append(unsigned int remainingBytes, BitInputReader& reader) {
    unsigned int toRead;
    if (remainingBytes > RESERVOIR_SIZE) {
        toRead = RESERVOIR_SIZE;
        unsigned int toSkip = remainingBytes - RESERVOIR_SIZE;
        reader.skipBytes(toSkip);
    } else {
        toRead = remainingBytes;
    }

    reader.extractBytes(&readBuffer[0], toRead);

    unsigned int remainingReservoir = RESERVOIR_SIZE - currentPos;
    unsigned int toCopy = std::min(toRead, remainingReservoir);
    std::copy(readBuffer.begin(), readBuffer.begin() + toCopy, reservoir.begin() + currentPos);

    if (toRead > remainingReservoir) {
        std::copy(readBuffer.begin() + remainingReservoir, readBuffer.begin() + toRead, reservoir.begin());
    }

    currentPos = (currentPos + toRead) % RESERVOIR_SIZE;
}

std::unique_ptr<BitInputReader> ByteReservoir::readerForPast(unsigned int nBytes) {
    unsigned int startPos;
    if (nBytes < currentPos) {
        startPos = currentPos - nBytes;
    } else {
        startPos = RESERVOIR_SIZE - (nBytes - currentPos);
    }

    ByteReservoirOperations ops(*this, startPos, nBytes);
    return std::make_unique<BasicBitReader<ByteReservoirOperations>>(ops);
}

void ByteReservoir::clear() { currentPos = 0; }
