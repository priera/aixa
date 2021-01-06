#include "Mp3Functions.h"

#include <unordered_map>
#include <vector>

namespace mp3Functions {

static std::unordered_map<unsigned char, unsigned int> BIT_RATE_DICTIONARY = {
    {1, 32},  {2, 40},  {3, 48},   {4, 56},   {5, 64},   {6, 80},   {7, 96},
    {8, 112}, {9, 128}, {10, 160}, {11, 192}, {12, 224}, {13, 256}, {14, 320}};

static std::vector<unsigned int> SAMPLING_FREQS = {44100, 48000, 32000};

bool seekToNextFrame(FileReader& f, FrameHeader& header) {
    unsigned char b;
    bool headerFound = false;
    bool headerStartRead = false;
    while (!headerFound && !f.ended()) {
        b = f.nextByte();

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

    if (f.ended()) return false;

    header = decodeHeader(f, b);
    return true;
}

FrameHeader decodeHeader(ByteReader& reader, unsigned char secondByte) {
    auto ret = FrameHeader();

    ret.version = (secondByte & 0x08) ? FrameHeader::Version::MPEG_1 : FrameHeader::Version::MPEG_2;
    ret.layer = static_cast<FrameHeader::Layer>((secondByte & 0x06) >> 1);
    ret.usesCRC = !(secondByte & 0x01);

    if (!(ret.version == FrameHeader::Version::MPEG_1 && ret.layer == FrameHeader::Layer::LAYER_3))
        throw std::runtime_error("Not supported MP3 format");

    unsigned char b = reader.nextByte();

    unsigned char bitRateIndex = (b & 0xF0) >> 4;
    if (bitRateIndex == 0 || bitRateIndex >= 15) throw std::runtime_error("Not supported MP3 format");
    ret.bitrate = BIT_RATE_DICTIONARY[bitRateIndex];

    std::size_t freqIndex = (b & 0x0A) >> 2;
    if (freqIndex > 2) throw std::runtime_error("Not supported MP3 format");
    ret.samplingFreq = SAMPLING_FREQS[freqIndex];

    ret.isPadded = b & 0x02;

    b = reader.nextByte();
    ret.mode = static_cast<FrameHeader::Mode>(b >> 6);

    ret.msStereo = b & 0x20;
    ret.intensityStereo = b & 0x10;

    return ret;
}

constexpr std::size_t REGIONS_NORMAL = 3;
constexpr std::size_t REGIONS_WINDOW_SWITCHING = 2;

void setRegionCountForGranule(GranuleChannelSideInfo& chGranule);

SideInformation decodeSideInformation(ByteReader& reader, const FrameHeader& header) {
    const auto channels = header.channels();

    auto sideInfo = SideInformation();

    sideInfo.mainDataBegin = reader.nextNBits(9);
    unsigned char bitsToSkip = (channels == 1) ? 5 : 3;
    reader.skipNBits(bitsToSkip);

    for (std::size_t i = 0; i < channels; i++) {
        sideInfo.scaleFactorSharing[i] = reader.nextNBits(4);
    }

    for (auto& granule : sideInfo.granules) {
        for (std::size_t ch = 0; ch < channels; ch++) {
            auto& chSideInfo = granule[ch];
            chSideInfo.part2_3_length = reader.nextNBits(12);
            chSideInfo.bigValues = reader.nextNBits(9);
            chSideInfo.globalGain = reader.nextNBits(8);
            chSideInfo.scaleFactorCompression = reader.nextNBits(4);
            chSideInfo.windowSwitching = reader.nextBit();

            if (chSideInfo.windowSwitching) {
                chSideInfo.blockType = static_cast<GranuleChannelSideInfo::BlockType>(reader.nextNBits(2));
                chSideInfo.mixedBlockFlag = reader.nextBit();
                for (std::size_t i = 0; i < REGIONS_WINDOW_SWITCHING; i++)
                    chSideInfo.tableSelect[i] = reader.nextNBits(5);
                for (std::size_t i = 0; i < NR_GAIN_WINDOWS; i++)
                    chSideInfo.subBlockGain[i] = reader.nextNBits(3);

                setRegionCountForGranule(chSideInfo);
            } else {
                chSideInfo.blockType = GranuleChannelSideInfo::BlockType::NORMAL;
                for (std::size_t i = 0; i < REGIONS_NORMAL; i++)
                    chSideInfo.tableSelect[i] = reader.nextNBits(5);
                chSideInfo.region0_count = reader.nextNBits(4);
                chSideInfo.region1_count = reader.nextNBits(3);
            }

            chSideInfo.preFlag = reader.nextBit();
            chSideInfo.scaleFactorScale = reader.nextBit();
            chSideInfo.count1TableSelect = reader.nextBit();
        }
    }

    return sideInfo;
}

void setRegionCountForGranule(GranuleChannelSideInfo& chGranule) {
    if (chGranule.blockType == GranuleChannelSideInfo::BlockType::NORMAL)
        throw std::runtime_error("Not valid block type");

    if (chGranule.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT && !chGranule.mixedBlockFlag) {
        chGranule.region0_count = 8;
    } else {
        chGranule.region0_count = 7;
    }

    chGranule.region1_count = 20 - chGranule.region0_count;
}

}  // namespace mp3Functions
