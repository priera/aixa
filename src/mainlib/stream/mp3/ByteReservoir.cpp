#include "ByteReservoir.h"

ByteReservoir::ByteReservoir() :
    reservoir(RESERVOIR_SIZE, 0),
    readBuffer(RESERVOIR_SIZE),
    currentPos(0),
    capacity(0) {}

void ByteReservoir::append(unsigned int remainingBytes, ByteReader& reader) {
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
        toCopy = toRead - remainingReservoir;
        std::copy(readBuffer.begin() + remainingReservoir, readBuffer.begin() + remainingReservoir + toCopy,
                  reservoir.begin());
    }

    currentPos = (currentPos + toRead) % RESERVOIR_SIZE;
    capacity += toRead;
}

void ByteReservoir::advance(unsigned int nBytes) { currentPos = (currentPos + nBytes) % RESERVOIR_SIZE; }

std::vector<char> ByteReservoir::extract(unsigned int nBytes) {
    std::vector<char> ret(nBytes);
    if (nBytes <= currentPos) {
        std::copy(reservoir.begin() + currentPos - nBytes, reservoir.begin() + currentPos, ret.begin());
    } else {
        auto diff = nBytes - currentPos;
        std::copy(reservoir.end() - diff, reservoir.end(), ret.begin());
        std::copy(reservoir.begin(), reservoir.begin() + currentPos, ret.begin() + diff);
    }

    return ret;
}

std::unique_ptr<ByteReader> ByteReservoir::readerForPast(unsigned int nBytes) {
    unsigned int startPos;
    if (nBytes < currentPos) {
        startPos = currentPos - nBytes;
    } else {
        startPos = RESERVOIR_SIZE - (nBytes - currentPos);
    }

    ByteReservoirOperations ops(*this, startPos, nBytes);
    return std::make_unique<BasicByteReader<ByteReservoirOperations>>(ops);
}
