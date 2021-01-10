#include "MainDataReader.h"

void MainDataReader::startFrame(unsigned int mainDataBegin) {
    if (mainDataBegin > 0) {
        reservoirReader = reservoir.readerForPast(mainDataBegin);
        currentReader = reservoirReader.get();
    } else {
        currentReader = inStream;
    }
    inStreamRead = inStream->tellg();
}

void MainDataReader::frameEnded(unsigned int remainingBits) {
    if (remainingBits % 8 != 0) {
        throw std::runtime_error("Remaining bytes is not an integer number of bytes");
    }

    reservoir.append(remainingBits / 8, *inStream);
}

void MainDataReader::advanceReservoir(unsigned int nBytes) { reservoir.advance(nBytes); }
