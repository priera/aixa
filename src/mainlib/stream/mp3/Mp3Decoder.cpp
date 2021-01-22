#include "Mp3Decoder.h"

#include <vector>

std::vector<unsigned int> Mp3Decoder::bitRateList = {32,  40,  48,  56,  64,  80,  96,
                                                     112, 128, 160, 192, 224, 256, 320};

std::vector<unsigned int> Mp3Decoder::samplingFreqs = {44100, 48000, 32000};

std::vector<std::vector<unsigned char>> Mp3Decoder::scaleFactorsCompression = {
    {0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4},
    {0, 1, 2, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 3}};

std::vector<std::vector<unsigned int>> Mp3Decoder::scaleFactorBandsGroups = {{0, 6, 12}, {0, 6, 11, 16, 21}};

std::map<int, Mp3Decoder::BandIndexes> Mp3Decoder::samplingFreqBandIndexes = {
    {44100,
     {{0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90, 110, 134, 162, 196, 238, 288, 342, 418, 576},
      {0, 4, 8, 12, 16, 22, 30, 40, 52, 66, 84, 106, 136, 192}}},
    {48000,
     {{0, 4, 8, 12, 16, 20, 24, 30, 36, 42, 50, 60, 72, 88, 106, 128, 156, 190, 230, 276, 330, 384, 576},
      {0, 4, 8, 12, 16, 22, 28, 38, 50, 64, 80, 100, 126, 192}}},
    {32000,
     {{0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 54, 66, 82, 102, 126, 156, 194, 240, 296, 364, 448, 550, 576},
      {0, 4, 8, 12, 16, 22, 30, 42, 58, 78, 104, 138, 180, 192}}}};

Mp3Decoder::Mp3Decoder(std::unique_ptr<ByteReader> reader, std::unique_ptr<MainDataReader> mainDataReader,
                       std::unique_ptr<HuffmanSet> huffmanSet) :
    f(std::move(reader)),
    header(),
    bytesInHeaders(0),
    currentFrameSize(0),
    mainDataReader(std::move(mainDataReader)),
    huffmanSet(std::move(huffmanSet)),
    sideInfo(),
    mainDataContent() {}

bool Mp3Decoder::decodeNextFrame(FrameHeader& retHeader) {
    if (!seekToNextFrame()) {
        return false;
    }

    float byteRate = (static_cast<float>(header.bitrate * 1000) / 8);
    currentFrameSize = (SAMPLES_PER_FRAME * byteRate) / header.samplingFreq;
    currentFrameSize += (header.isPadded) ? 1 : 0;

    skipCRC();

    decodeSideInformation();

    decodeMainData();

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

void Mp3Decoder::decodeSideInformation() {
    const auto channels = header.channels();

    sideInfo = SideInformation();

    sideInfo.mainDataBegin = f->nextNBits(9);
    unsigned char bitsToSkip = (channels == 1) ? 5 : 3;
    f->skipNBits(bitsToSkip);

    for (std::size_t i = 0; i < channels; i++) {
        for (std::size_t group = 0; group < NR_SUB_BAND_GROUPS; group++)
            sideInfo.scaleFactorSharing[i][group] = f->nextBit();
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
                for (std::size_t i = 0; i < NR_SHORT_WINDOWS; i++)
                    chSideInfo.subBlockGain[i] = f->nextNBits(3);

                setRegionCountForGranule(chSideInfo);
            } else {
                chSideInfo.blockType = GranuleChannelSideInfo::BlockType::NORMAL;
                for (std::size_t i = 0; i < REGIONS_NORMAL_BLOCK; i++)
                    chSideInfo.tableSelect[i] = f->nextNBits(5);
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

void Mp3Decoder::decodeMainData() {
    mainDataReader->startFrame(sideInfo.mainDataBegin);

    const auto channels = header.channels();
    bool readingSecondGranule = false;
    for (unsigned int i = 0; i < NR_GRANULES; i++) {
        for (unsigned int channel = 0; channel < channels; channel++) {
            const auto& channelInfo = sideInfo.granules[i][channel];
            auto& channelContent = mainDataContent.granules[i][channel];
            readChannelScaleFactors(channelInfo, channelContent, channel, readingSecondGranule);
            entropyDecode(channelInfo, channelContent);
        }

        readingSecondGranule = true;
    }

    unsigned int pending = currentFrameSize - bytesInHeaders - mainDataReader->tellg();
    mainDataReader->frameEnded(pending * 8);
}

void Mp3Decoder::readChannelScaleFactors(const GranuleChannelSideInfo& channelSideInfo,
                                         GranuleChannelContent& channelContent, unsigned int channel,
                                         bool readingSecondGranule) {
    const unsigned char slen1 = scaleFactorsCompression[0][channelSideInfo.scaleFactorCompression];
    const unsigned char slen2 = scaleFactorsCompression[1][channelSideInfo.scaleFactorCompression];

    if (channelSideInfo.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        if (channelSideInfo.mixedBlockFlag)
            throw std::runtime_error("Not supported (yet) reading these scale factors");

        for (unsigned int group = 0; group < 2; group++) {
            const unsigned int subBandStart = scaleFactorBandsGroups[0][group];
            const unsigned int subBandEnd = scaleFactorBandsGroups[0][group + 1];
            unsigned char toRead = (group == 0) ? slen1 : slen2;
            for (unsigned int i = subBandStart; i < subBandEnd; i++) {
                for (unsigned int window = 0; window < NR_SHORT_WINDOWS; window++) {
                    channelContent.shortWindowScaleFactorBands[window][i] = mainDataReader->nextNBits(toRead);
                }
            }
        }
    } else {
        for (unsigned int group = 0; group < NR_SUB_BAND_GROUPS; group++) {
            if (!(readingSecondGranule && sideInfo.scaleFactorSharing[channel][group])) {
                const unsigned int subBandStart = scaleFactorBandsGroups[1][group];
                const unsigned int subBandEnd = scaleFactorBandsGroups[1][group + 1];
                unsigned char toRead = (group < 2) ? slen1 : slen2;
                for (unsigned int i = subBandStart; i < subBandEnd; i++) {
                    channelContent.longWindowScaleFactorBands[i] = mainDataReader->nextNBits(toRead);
                }
            }
        }
    }
}

void Mp3Decoder::entropyDecode(const GranuleChannelSideInfo& channelInfo, GranuleChannelContent& content) {
    if (channelInfo.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        throw std::runtime_error("TODO Huffman decoding of short windows not supported");
    }

    if (channelInfo.bigValues == 0) {
        content.freqLines.assign(NR_GRANULE_FREQ_LINES, 0);
        return;
    }

    const auto& indexesTable = samplingFreqBandIndexes[header.samplingFreq].longWindow;
    unsigned int region0Samples = indexesTable[channelInfo.region0_count + 1];
    unsigned int region1Samples = indexesTable[channelInfo.region0_count + channelInfo.region1_count + 2];

    unsigned int freqLinesDecoded = 0;
    while (freqLinesDecoded < 2 * channelInfo.bigValues) {
        std::size_t tableIndex;
        if (freqLinesDecoded < region0Samples) {
            tableIndex = 0;
        } else if (freqLinesDecoded < region1Samples) {
            tableIndex = 1;
        } else {
            tableIndex = 2;
        }
        const auto& table = huffmanSet->getTable(channelInfo.tableSelect[tableIndex]);
        int x, y;
        table.decode(*mainDataReader, x, y);
        content.freqLines.push_back(x);
        content.freqLines.push_back(y);
        freqLinesDecoded += 2;
    }
}
