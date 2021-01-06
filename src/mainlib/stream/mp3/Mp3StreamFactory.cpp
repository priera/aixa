#include "Mp3StreamFactory.h"

#include <mainlib/stream/FileReader.h>

#include "Mp3Functions.h"

std::shared_ptr<Stream> Mp3StreamFactory::probe() {
    FileReader f(streamPath);

    auto header = FrameHeader();
    mp3Functions::seekToNextFrame(f, header);

    if (header.usesCRC) {
        f.nextByte();
        f.nextByte();
    }

    auto sideInfo = mp3Functions::decodeSideInformation(f, header);

    return nullptr;
}
