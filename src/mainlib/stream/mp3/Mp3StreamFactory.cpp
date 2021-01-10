#include "Mp3StreamFactory.h"

#include <mainlib/stream/in/FileReader.h>

#include "Mp3Decoder.h"

std::shared_ptr<Stream> Mp3StreamFactory::probe() {
    auto reader = std::unique_ptr<ByteReader>(FileReader::buildForFile(streamPath));
    Mp3Decoder decoder(std::move(reader));

    auto header = FrameHeader();
    decoder.decodeNextFrame(header);
    decoder.decodeNextFrame(header);
    decoder.decodeNextFrame(header);

    return nullptr;
}
