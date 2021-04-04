#include "Mp3Decoder.h"

#include <vector>

static int frameNum = 0;

Mp3Decoder::Mp3Decoder(std::unique_ptr<BitInputReader> reader,
                       std::unique_ptr<FrameDecoder> frameDecoder,
                       std::unique_ptr<FrameSynthesizer> frameSynthesizer) :
    reader(std::move(reader)),
    frameDecoder(std::move(frameDecoder)), frameSynthesizer(std::move(frameSynthesizer)) {}

bool Mp3Decoder::decodeNextFrame() {
    FrameStartToken t;
    if (!seekToNextFrame(t)) {
        return false;
    }

    const auto& frame = frameDecoder->decode(t);
    frameSynthesizer->synthesize(frame);

    return true;
}

bool Mp3Decoder::seekToNextFrame(FrameStartToken& tok) {
    if (frameNum >= 195) {
        char a = 3;
    }
    unsigned char b;
    bool headerFound = false;
    bool headerStartRead = false;
    while (!headerFound && !reader->ended()) {
        b = reader->nextByte();

        if (headerStartRead) {
            if ((b & 0xF0) == 0xF0) {
                headerFound = true;
            } else {
                headerStartRead = false;
            }
        } else if (b == 0xFF) {
            headerStartRead = true;
        }
    }

    if (reader->ended())
        return false;

    tok = b;
    frameNum++;
    return true;
}
