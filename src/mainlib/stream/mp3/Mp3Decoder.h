#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H

#include <mainlib/stream/ByteReader.h>
#include <mainlib/stream/FileReader.h>

#include <unordered_map>
#include <vector>

#include "FrameHeader.h"

class Mp3Decoder {
public:
    explicit Mp3Decoder(const std::string& path);
    virtual ~Mp3Decoder() = default;

    bool decodeNextFrame(FrameHeader& header);

private:
    static constexpr unsigned int SAMPLES_PER_FRAME = 1152;

    static constexpr std::size_t REGIONS_NORMAL = 3;
    static constexpr std::size_t REGIONS_WINDOW_SWITCHING = 2;

    static std::unordered_map<unsigned char, unsigned int> bitRateDictionary;
    static std::vector<unsigned int> samplingFreqs;

    bool seekToNextFrame();
    void decodeHeader(unsigned char secondByte);
    SideInformation decodeSideInformation(const FrameHeader& header);
    void setRegionCountForGranule(GranuleChannelSideInfo& chGranule);

    FileReader f;
    FrameHeader header;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
