#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H

#include <mainlib/stream/in/BasicBitReader.h>
#include <mainlib/stream/mp3/frame/FrameDecoder.h>
#include <mainlib/stream/mp3/frame/FrameSynthesizer.h>

#include <map>
#include <memory>
#include <vector>

#include "types.h"

class Mp3Decoder {
public:
    Mp3Decoder(std::unique_ptr<BitInputReader> reader,
               std::unique_ptr<FrameDecoder> decoder,
               std::unique_ptr<FrameSynthesizer> frameSynthesizer);

    virtual ~Mp3Decoder() = default;

    bool decodeNextFrame(FrameHeader& header);
    void resetToBeginning();

private:
    bool seekToNextFrame(FrameStartToken& tok);

    std::unique_ptr<BitInputReader> reader;
    std::unique_ptr<FrameDecoder> frameDecoder;
    std::unique_ptr<FrameSynthesizer> frameSynthesizer;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
