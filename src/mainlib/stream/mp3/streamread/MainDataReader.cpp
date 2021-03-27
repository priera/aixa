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

void MainDataReader::frameEnded(unsigned int remainingBytes) {
    byteAlign();
    reservoir.append(remainingBytes, *inStream);
    currentReader = inStream.get();
}
