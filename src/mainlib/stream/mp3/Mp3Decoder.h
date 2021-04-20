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

    bool seekToNextFrame();

    void decodeFrame(FrameSamples& samples) {
        FrameHeader tmp;
        decodeFrame(tmp, samples);
    }

    void decodeFrame(FrameHeader& header, FrameSamples& samples);

    bool probeNextFrame(FrameHeader& header) {
        auto samples = FrameSamples();
        auto ret = seekToNextFrame();
        // Will throw is frame is ill-formed
        decodeFrame(header, samples);
        return ret;
    }

    void resetToBeginning();

    std::size_t framesDecoded() const { return framesProcessed; }

private:
    std::unique_ptr<BitInputReader> reader;
    std::unique_ptr<FrameDecoder> frameDecoder;
    std::unique_ptr<FrameSynthesizer> frameSynthesizer;

    std::size_t framesProcessed;
    FrameStartToken tok;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
