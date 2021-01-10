#include "Mp3StreamFactory.h"

#include <mainlib/stream/in/FileReader.h>

#include "Mp3Decoder.h"

std::shared_ptr<Stream> Mp3StreamFactory::probe() {
    auto reader = std::make_unique<FileReader>(streamPath);
    auto mainDataReader = std::make_unique<MainDataReader>(*reader);
    Mp3Decoder decoder(std::move(reader), std::move(mainDataReader));

    auto header = FrameHeader();
    decoder.decodeNextFrame(header);
    decoder.decodeNextFrame(header);
    decoder.decodeNextFrame(header);

    return nullptr;
}
