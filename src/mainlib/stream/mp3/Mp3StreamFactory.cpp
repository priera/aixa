#include "Mp3StreamFactory.h"

#include <mainlib/stream/in/FileReader.h>
#include <mainlib/stream/mp3/exception/exceptions.h>
#include <mainlib/stream/mp3/frame/FrameSynthesizerFactory.h>
#include <mainlib/stream/mp3/huffman/Huffman.h>
#include <mainlib/stream/mp3/huffman/HuffmanParser.h>

#include <iostream>

#include "Mp3Decoder.h"
#include "Mp3Stream.h"

std::shared_ptr<Stream> Mp3StreamFactory::probe() {
    auto decoder = std::unique_ptr<Mp3Decoder>(buildDecoder());

    auto header = FrameHeader();
    probeWithDecoder(*decoder, header);

    decoder->resetToBeginning();

    return std::make_shared<Mp3Stream>(header, std::move(decoder));
}

Mp3Decoder* Mp3StreamFactory::buildDecoder() {
    auto reader = std::make_unique<FileReader>(streamPath);
    auto huffmanSet_p = HuffmanParser("./data/mp3/huffdec").build();
    auto huffmanSet = std::unique_ptr<HuffmanSet>(huffmanSet_p);
    auto mainDataReader = std::make_unique<MainDataReader>(std::move(reader));

    auto frameDecoder = std::make_unique<FrameDecoder>(*mainDataReader, std::move(huffmanSet));

    auto frameSynthesizerFactory = FrameSynthesizerFactory("./data/mp3/dewindow");
    auto frameSynthesizer = std::unique_ptr<FrameSynthesizer>(frameSynthesizerFactory.build());
    return new Mp3Decoder(std::move(mainDataReader), std::move(frameDecoder), std::move(frameSynthesizer));
}

void Mp3StreamFactory::probeWithDecoder(Mp3Decoder& decoder, FrameHeader& header) {
    bool streamEnded = false;

    // Decode first few frames. Since is possible that synchronization word will appear in ID3 tag info,
    // first frame will be omitted, along with its decoding errors
    // I assume the file does not change its encoding format, so the information from the last decoded frame
    // is used to configure the stream
    try {
        streamEnded = decoder.decodeNextFrame(header);
    } catch (const MP3StreamException& e) {
        std::cerr << "Error when decoding first frame. Exception was:\n" << e.what() << std::endl;
    }

    std::size_t probingFrame = 0;
    while (!streamEnded && probingFrame < PROBING_FRAMES) {
        streamEnded = decoder.decodeNextFrame(header);
        probingFrame++;
    }

    if (streamEnded)
        throw InvalidStream(streamPath);
}
