#include "Mp3StreamFactory.h"

#include "Mp3Decoder.h"

std::shared_ptr<Stream> Mp3StreamFactory::probe() {
    Mp3Decoder decoder(streamPath);

    auto header = FrameHeader();
    decoder.decodeNextFrame(header);

    return nullptr;
}
