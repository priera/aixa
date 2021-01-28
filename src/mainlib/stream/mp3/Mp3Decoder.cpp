#include "Mp3Decoder.h"

#include <iostream>
#include <vector>

static int i = 0;

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

Mp3Decoder::Mp3Decoder(std::unique_ptr<MainDataReader> reader, std::unique_ptr<HuffmanSet> huffmanSet) :
    header(),
    bytesInHeaders(0),
    currentFrameSize(0),
    reader(std::move(reader)),
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
    i++;
    return true;
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

    if (reader->ended()) return false;

    decodeHeader(b);
    return true;
}

void Mp3Decoder::decodeHeader(unsigned char secondByte) {
    header.version = (secondByte & 0x08) ? FrameHeader::Version::MPEG_1 : FrameHeader::Version::MPEG_2;
    header.layer = static_cast<FrameHeader::Layer>((secondByte & 0x06) >> 1);
    header.usesCRC = !(secondByte & 0x01);

    if (!(header.version == FrameHeader::Version::MPEG_1 && header.layer == FrameHeader::Layer::LAYER_3))
        throw std::runtime_error("Not supported MP3 format");

    unsigned char b = reader->nextByte();

    unsigned char bitRateIndex = (b & 0xF0) >> 4;
    if (bitRateIndex == 0 || bitRateIndex >= 15) throw std::runtime_error("Not supported MP3 format");
    header.bitrate = bitRateList[bitRateIndex - 1];

    std::size_t freqIndex = (b & 0x0A) >> 2;
    if (freqIndex > 2) throw std::runtime_error("Not supported MP3 format");
    header.samplingFreq = samplingFreqs[freqIndex];

    header.isPadded = b & 0x02;

    b = reader->nextByte();
    header.mode = static_cast<FrameHeader::Mode>(b >> 6);

    header.msStereo = b & 0x20;
    header.intensityStereo = b & 0x10;

    bytesInHeaders = 4;
}

void Mp3Decoder::skipCRC() {
    if (header.usesCRC) {
        reader->nextByte();
        reader->nextByte();
        bytesInHeaders += 2;
    }
}

void Mp3Decoder::decodeSideInformation() {
    const auto channels = header.channels();

    sideInfo = SideInformation();

    sideInfo.mainDataBegin = reader->nextNBits(9);
    unsigned char bitsToSkip = (channels == 1) ? 5 : 3;
    reader->skipNBits(bitsToSkip);

    for (std::size_t i = 0; i < channels; i++) {
        for (std::size_t group = 0; group < NR_SUB_BAND_GROUPS; group++)
            sideInfo.scaleFactorSharing[i][group] = reader->nextBit();
    }

    for (auto& granule : sideInfo.granules) {
        for (std::size_t ch = 0; ch < channels; ch++) {
            auto& chSideInfo = granule[ch];
            chSideInfo.part2_3_length = reader->nextNBits(12);
            chSideInfo.bigValues = reader->nextNBits(9);
            chSideInfo.globalGain = reader->nextNBits(8);
            chSideInfo.scaleFactorCompression = reader->nextNBits(4);
            chSideInfo.windowSwitching = reader->nextBit();

            if (chSideInfo.windowSwitching) {
                chSideInfo.blockType = static_cast<GranuleChannelSideInfo::BlockType>(reader->nextNBits(2));
                chSideInfo.mixedBlockFlag = reader->nextBit();
                for (std::size_t i = 0; i < REGIONS_WINDOW_SWITCHING; i++)
                    chSideInfo.tableSelect[i] = reader->nextNBits(5);
                for (std::size_t i = 0; i < NR_SHORT_WINDOWS; i++)
                    chSideInfo.subBlockGain[i] = reader->nextNBits(3);

                setRegionCountForGranule(chSideInfo);
            } else {
                chSideInfo.blockType = GranuleChannelSideInfo::BlockType::NORMAL;
                for (std::size_t i = 0; i < REGIONS_NORMAL_BLOCK; i++)
                    chSideInfo.tableSelect[i] = reader->nextNBits(5);
                chSideInfo.region0_count = reader->nextNBits(4);
                chSideInfo.region1_count = reader->nextNBits(3);
            }

            chSideInfo.preFlag = reader->nextBit();
            chSideInfo.scaleFactorScale = reader->nextBit();
            chSideInfo.count1TableSelect = reader->nextBit();
        }
    }

    auto sideInfoSize = (channels == 1) ? SIDE_INFO_SIZE_MONO : SIDE_INFO_SIZE_DUAL;
    bytesInHeaders += sideInfoSize;
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
    if (i >= 175) {
        char a = 3;
    }
    reader->startFrame(sideInfo.mainDataBegin);

    const auto channels = header.channels();
    bool readingSecondGranule = false;
    for (unsigned int i = 0; i < NR_GRANULES; i++) {
        for (unsigned int channel = 0; channel < channels; channel++) {
            const auto& channelInfo = sideInfo.granules[i][channel];
            auto& channelContent = mainDataContent.granules[i][channel];
            auto channelStart = reader->bitsRead();
            readChannelScaleFactors(channelInfo, channelContent, channel, readingSecondGranule);
            entropyDecode(channelInfo, channelStart, channelContent);
        }

        readingSecondGranule = true;
    }

    unsigned int pending = currentFrameSize - bytesInHeaders - reader->streamConsumedBytes();
    reader->frameEnded(pending * 8);
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
                    channelContent.shortWindowScaleFactorBands[window][i] = reader->nextNBits(toRead);
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
                    channelContent.longWindowScaleFactorBands[i] = reader->nextNBits(toRead);
                }
            }
        }
    }
}

void Mp3Decoder::entropyDecode(const GranuleChannelSideInfo& channelInfo, unsigned long channelStart,
                               GranuleChannelContent& content) {
    unsigned int region0Samples;
    unsigned int region1Samples;

    if (channelInfo.windowSwitching &&
        channelInfo.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        region0Samples = 36; /* sfb[9/3]*3=36 */
        region1Samples = SAMPLES_PER_GRANULE;
    } else {
        const auto& indexesTable = samplingFreqBandIndexes[header.samplingFreq].longWindow;
        region0Samples = indexesTable[channelInfo.region0_count + 1];
        region1Samples = indexesTable[channelInfo.region0_count + channelInfo.region1_count + 2];
    }

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
        table.decodeBigValues(*reader, x, y);
        content.freqLines.push_back(x);
        content.freqLines.push_back(y);
        freqLinesDecoded += 2;
    }

    const auto& count1Table = huffmanSet->getTable(32 + channelInfo.count1TableSelect);
    while (reader->bitsRead() < channelStart + channelInfo.part2_3_length) {
        int x, y, v, w;
        count1Table.decodeCount1(*reader, x, y, v, w);
        content.freqLines.push_back(v);
        content.freqLines.push_back(w);
        content.freqLines.push_back(x);
        content.freqLines.push_back(y);
        freqLinesDecoded += 4;
    }

    for (; freqLinesDecoded < NR_GRANULE_FREQ_LINES; freqLinesDecoded++) {
        content.freqLines.push_back(0);
    }
}
