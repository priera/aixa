#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H

#include <mainlib/stream/ByteReader.h>
#include <mainlib/stream/FileReader.h>

#include <vector>

#include "ByteReservoir.h"
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

    static constexpr unsigned int SIDE_INFO_SIZE_MONO = 17;
    static constexpr unsigned int SIDE_INFO_SIZE_DUAL = 32;

    static std::vector<unsigned int> bitRateList;
    static std::vector<unsigned int> samplingFreqs;

    bool seekToNextFrame();
    void decodeHeader(unsigned char secondByte);
    SideInformation decodeSideInformation(const FrameHeader& header);
    void setRegionCountForGranule(GranuleChannelSideInfo& chGranule);

    FileReader f;
    FrameHeader header;
    ByteReservoir reservoir;
    unsigned int bytesRead;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
