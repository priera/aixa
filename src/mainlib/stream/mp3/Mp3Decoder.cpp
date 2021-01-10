#include "Mp3Decoder.h"

#include <vector>

std::vector<unsigned int> Mp3Decoder::bitRateList = {32,  40,  48,  56,  64,  80,  96,
                                                     112, 128, 160, 192, 224, 256, 320};

std::vector<unsigned int> Mp3Decoder::samplingFreqs = {44100, 48000, 32000};

Mp3Decoder::Mp3Decoder(std::unique_ptr<ByteReader> reader, std::unique_ptr<MainDataReader> mainDataReader) :
    f(std::move(reader)),
    header(),
    bytesInHeaders(0),
    mainDataReader(std::move(mainDataReader)) {}

bool Mp3Decoder::decodeNextFrame(FrameHeader& retHeader) {
    if (!seekToNextFrame()) {
        return false;
    }

    float byteRate = (static_cast<float>(header.bitrate * 1000) / 8);
    unsigned int frameSize = (SAMPLES_PER_FRAME * byteRate) / header.samplingFreq;
    frameSize += (header.isPadded) ? 1 : 0;

    skipCRC();

    auto sideInfo = decodeSideInformation(header);
    mainDataReader->startFrame(sideInfo.mainDataBegin);

    unsigned int pending = frameSize - bytesInHeaders - mainDataReader->tellg();
    mainDataReader->frameEnded(pending * 8);

    retHeader = header;
    return true;
}

bool Mp3Decoder::seekToNextFrame() {
    unsigned char b;
    bool headerFound = false;
    bool headerStartRead = false;
    while (!headerFound && !f->ended()) {
        b = f->nextByte();

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

    if (f->ended()) return false;

    decodeHeader(b);
    return true;
}

void Mp3Decoder::decodeHeader(unsigned char secondByte) {
    header.version = (secondByte & 0x08) ? FrameHeader::Version::MPEG_1 : FrameHeader::Version::MPEG_2;
    header.layer = static_cast<FrameHeader::Layer>((secondByte & 0x06) >> 1);
    header.usesCRC = !(secondByte & 0x01);

    if (!(header.version == FrameHeader::Version::MPEG_1 && header.layer == FrameHeader::Layer::LAYER_3))
        throw std::runtime_error("Not supported MP3 format");

    unsigned char b = f->nextByte();

    unsigned char bitRateIndex = (b & 0xF0) >> 4;
    if (bitRateIndex == 0 || bitRateIndex >= 15) throw std::runtime_error("Not supported MP3 format");
    header.bitrate = bitRateList[bitRateIndex - 1];

    std::size_t freqIndex = (b & 0x0A) >> 2;
    if (freqIndex > 2) throw std::runtime_error("Not supported MP3 format");
    header.samplingFreq = samplingFreqs[freqIndex];

    header.isPadded = b & 0x02;

    b = f->nextByte();
    header.mode = static_cast<FrameHeader::Mode>(b >> 6);

    header.msStereo = b & 0x20;
    header.intensityStereo = b & 0x10;

    bytesInHeaders = 4;
}

void Mp3Decoder::skipCRC() {
    if (header.usesCRC) {
        f->nextByte();
        f->nextByte();
        bytesInHeaders += 2;
        mainDataReader->advanceReservoir(2);
    }
}

SideInformation Mp3Decoder::decodeSideInformation(const FrameHeader& header) {
    const auto channels = header.channels();

    auto sideInfo = SideInformation();

    sideInfo.mainDataBegin = f->nextNBits(9);
    unsigned char bitsToSkip = (channels == 1) ? 5 : 3;
    f->skipNBits(bitsToSkip);

    for (std::size_t i = 0; i < channels; i++) {
        sideInfo.scaleFactorSharing[i] = f->nextNBits(4);
    }

    for (auto& granule : sideInfo.granules) {
        for (std::size_t ch = 0; ch < channels; ch++) {
            auto& chSideInfo = granule[ch];
            chSideInfo.part2_3_length = f->nextNBits(12);
            chSideInfo.bigValues = f->nextNBits(9);
            chSideInfo.globalGain = f->nextNBits(8);
            chSideInfo.scaleFactorCompression = f->nextNBits(4);
            chSideInfo.windowSwitching = f->nextBit();

            if (chSideInfo.windowSwitching) {
                chSideInfo.blockType = static_cast<GranuleChannelSideInfo::BlockType>(f->nextNBits(2));
                chSideInfo.mixedBlockFlag = f->nextBit();
                for (std::size_t i = 0; i < REGIONS_WINDOW_SWITCHING; i++)
                    chSideInfo.tableSelect[i] = f->nextNBits(5);
                for (std::size_t i = 0; i < NR_GAIN_WINDOWS; i++)
                    chSideInfo.subBlockGain[i] = f->nextNBits(3);

                setRegionCountForGranule(chSideInfo);
            } else {
                chSideInfo.blockType = GranuleChannelSideInfo::BlockType::NORMAL;
                for (std::size_t i = 0; i < REGIONS_NORMAL; i++) chSideInfo.tableSelect[i] = f->nextNBits(5);
                chSideInfo.region0_count = f->nextNBits(4);
                chSideInfo.region1_count = f->nextNBits(3);
            }

            chSideInfo.preFlag = f->nextBit();
            chSideInfo.scaleFactorScale = f->nextBit();
            chSideInfo.count1TableSelect = f->nextBit();
        }
    }

    auto sideInfoSize = (channels == 1) ? SIDE_INFO_SIZE_MONO : SIDE_INFO_SIZE_DUAL;
    bytesInHeaders += sideInfoSize;
    mainDataReader->advanceReservoir(sideInfoSize);

    return sideInfo;
}

void Mp3Decoder::setRegionCountForGranule(GranuleChannelSideInfo& chGranule) {
    if (chGranule.blockType == GranuleChannelSideInfo::BlockType::NORMAL)
        throw std::runtime_error("Not valid block type");

    if (chGranule.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT && !chGranule.mixedBlockFlag) {
        chGranule.region0_count = 8;
    } else {
        chGranule.region0_count = 7;
    }

    chGranule.region1_count = 20 - chGranule.region0_count;
}
