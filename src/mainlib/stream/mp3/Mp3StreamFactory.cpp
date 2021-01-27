#include "Mp3StreamFactory.h"

#include <mainlib/stream/in/FileReader.h>
#include <mainlib/stream/mp3/huffman/Huffman.h>
#include <mainlib/stream/mp3/huffman/HuffmanParser.h>

#include <iostream>

#include "Mp3Decoder.h"

std::shared_ptr<Stream> Mp3StreamFactory::probe() {
    auto reader = std::make_unique<FileReader>(streamPath);
    auto huffmanSet_p = HuffmanParser("./data/mp3/huffdec").build();
    auto huffmanSet = std::unique_ptr<HuffmanSet>(huffmanSet_p);
    auto mainDataReader = std::make_unique<MainDataReader>(std::move(reader));
    auto decoder = Mp3Decoder(std::move(mainDataReader), std::move(huffmanSet));

    auto header = FrameHeader();
    for (int i = 0; i < 177; i++) {
        std::cout << "Decoding frame " << i << std::endl;

        if (i >= 175) {
            char a = 3;
        }
        if (!decoder.decodeNextFrame(header)) {
            std::cout << "Broken" << std::endl;
            break;
        }
    }

    return nullptr;
}
