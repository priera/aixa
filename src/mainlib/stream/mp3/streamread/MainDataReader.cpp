#include "MainDataReader.h"

void MainDataReader::startFrame(unsigned int mainDataBegin) {
    if (mainDataBegin > 0) {
        reservoirReader = reservoir.readerForPast(mainDataBegin);
        currentReader = reservoirReader.get();
    } else {
        currentReader = inStream.get();
    }
    inStreamAtStartFrame = inStream->bitsRead() / S_BYTE;
    frameBitsRead = 0;
}

void MainDataReader::frameEnded(unsigned int frameSize, unsigned int bytesInHeaders) {
    byteAlign();
    auto inStreamConsumedBytes = (inStream->bitsRead() / S_BYTE) - inStreamAtStartFrame;
    unsigned int pending = frameSize - bytesInHeaders - inStreamConsumedBytes;
    reservoir.append(pending, *inStream);
    currentReader = inStream.get();
}
