#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H

#include <mainlib/stream/in/BasicByteReader.h>

#include <memory>
#include <vector>

#include "ByteReservoir.h"
#include "FrameHeader.h"
#include "MainDataReader.h"

class Mp3Decoder {
public:
    Mp3Decoder(std::unique_ptr<ByteReader> reader, std::unique_ptr<MainDataReader> mainDataReader);

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
    void skipCRC();
    SideInformation decodeSideInformation(const FrameHeader& header);
    void setRegionCountForGranule(GranuleChannelSideInfo& chGranule);

    std::unique_ptr<ByteReader> f;
    FrameHeader header;
    std::unique_ptr<MainDataReader> mainDataReader;
    unsigned int bytesInHeaders;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
