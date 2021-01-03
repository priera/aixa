#include "Mp3StreamFactory.h"

#include <mainlib/stream/FileReader.h>

#include "Mp3Functions.h"

std::shared_ptr<Stream> Mp3StreamFactory::probe() {
    FileReader f(streamPath);

    auto firstFrame = mp3Functions::skipID3data(f);
    if (firstFrame.usesCRC) {
        f.nextByte();
        f.nextByte();
    }

    return nullptr;
}
