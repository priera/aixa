#include "Mp3Decoder.h"

#include <vector>

Mp3Decoder::Mp3Decoder(std::unique_ptr<BitInputReader> reader,
                       std::unique_ptr<FrameDecoder> frameDecoder,
                       std::unique_ptr<FrameSynthesizer> frameSynthesizer) :
    reader(std::move(reader)),
    frameDecoder(std::move(frameDecoder)), frameSynthesizer(std::move(frameSynthesizer)), framesProcessed(0),
    tok() {}

void Mp3Decoder::decodeFrame(FrameHeader& header, FrameSamples& samples) {
    const auto& frame = frameDecoder->decode(tok);
    samples = frameSynthesizer->synthesize(frame);

    framesProcessed++;
    header = frame.header;
}

bool Mp3Decoder::seekToNextFrame() {
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
    return true;
}

void Mp3Decoder::resetToBeginning() {
    reader->seekToBeginning();
    frameSynthesizer->clearState();
    framesProcessed = 0;
}
