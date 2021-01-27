#include "MainDataReader.h"

void MainDataReader::startFrame(unsigned int mainDataBegin) {
    if (mainDataBegin > 0) {
        reservoirReader = reservoir.readerForPast(mainDataBegin);
        currentReader = reservoirReader.get();
    } else {
        currentReader = inStream.get();
    }
    inStreamRead = inStream->bitsRead() / S_BYTE;
    myBitsRead = 0;
}

void MainDataReader::frameEnded(unsigned int remainingBits) {
    if (remainingBits % S_BYTE != 0) {
        throw std::runtime_error("Remaining bytes is not an integer number of bytes");
    }

    reservoir.append(remainingBits / S_BYTE, *inStream);
    currentReader = inStream.get();
}
