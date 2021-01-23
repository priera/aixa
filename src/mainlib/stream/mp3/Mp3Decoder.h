#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H

#include <mainlib/stream/in/BasicByteReader.h>
#include <mainlib/stream/mp3/huffman/Huffman.h>
#include <mainlib/stream/mp3/huffman/HuffmanSet.h>

#include <map>
#include <memory>
#include <vector>

#include "ByteReservoir.h"
#include "MainDataReader.h"
#include "types.h"

class Mp3Decoder {
public:
    Mp3Decoder(std::unique_ptr<MainDataReader> reader, std::unique_ptr<HuffmanSet> huffmanSet);

    virtual ~Mp3Decoder() = default;

    bool decodeNextFrame(FrameHeader& header);

private:
    struct BandIndexes {
        std::vector<unsigned int> longWindow;
        std::vector<unsigned int> shortWindow;
    };

    static constexpr unsigned int SAMPLES_PER_FRAME = 1152;

    static constexpr std::size_t REGIONS_NORMAL_BLOCK = 3;
    static constexpr std::size_t REGIONS_WINDOW_SWITCHING = 2;

    static constexpr unsigned int SIDE_INFO_SIZE_MONO = 17;
    static constexpr unsigned int SIDE_INFO_SIZE_DUAL = 32;

    static std::vector<unsigned int> bitRateList;
    static std::vector<unsigned int> samplingFreqs;
    static std::vector<std::vector<unsigned int>> scaleFactorBandsGroups;
    static std::vector<std::vector<unsigned char>> scaleFactorsCompression;
    static std::map<int, BandIndexes> samplingFreqBandIndexes;

    bool seekToNextFrame();
    void decodeHeader(unsigned char secondByte);
    void skipCRC();
    void decodeSideInformation();
    void setRegionCountForGranule(GranuleChannelSideInfo& chGranule);

    void decodeMainData();
    void readChannelScaleFactors(const GranuleChannelSideInfo& channelSideInfo,
                                 GranuleChannelContent& channelContent, unsigned int channel,
                                 bool readingSecondGranule);
    void entropyDecode(const GranuleChannelSideInfo& channelInfo, GranuleChannelContent& content);

    FrameHeader header;
    std::unique_ptr<MainDataReader> reader;
    std::unique_ptr<HuffmanSet> huffmanSet;
    unsigned int bytesInHeaders;
    unsigned int currentFrameSize;
    SideInformation sideInfo;
    MainDataContent mainDataContent;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3DECODER_H
