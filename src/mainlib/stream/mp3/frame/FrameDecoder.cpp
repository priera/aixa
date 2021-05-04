#include "FrameDecoder.h"

#include <mainlib/stream/in/sizes.h>
#include <mainlib/stream/mp3/exception/exceptions.h>

std::vector<std::vector<unsigned char>> FrameDecoder::scaleFactorsCompression = {
    {0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4},
    {0, 1, 2, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 3}};

std::vector<std::vector<unsigned int>> FrameDecoder::scaleFactorBandsGroups = {{0, 6, 12},
                                                                               {0, 6, 11, 16, 21}};

const Frame& FrameDecoder::decode(FrameStartToken token) {
    bytesInHeaders = 0;

    decodeHeader(token);

    computeFrameSize();

    skipCRC();

    decodeSideInformation();

    decodeMainData();

    return frame;
}

void FrameDecoder::computeFrameSize() {
    const auto& header = frame.header;
    float byteRate = (static_cast<float>(header.bitrate) / S_BYTE);
    unsigned int frameSize = (NR_FRAME_SAMPLES * byteRate) / header.samplingFreq;
    frameSize += (header.isPadded) ? 1 : 0;

    frame.frameSize = frameSize;
}

void FrameDecoder::decodeHeader(FrameStartToken tok) {
    auto& header = frame.header;

    header.version = (tok & 0x08) ? FrameHeader::Version::MPEG_1 : FrameHeader::Version::MPEG_2;
    header.layer = static_cast<FrameHeader::Layer>((tok & 0x06) >> 1);
    header.usesCRC = !(tok & 0x01);

    if (!(header.version == FrameHeader::Version::MPEG_1 && header.layer == FrameHeader::Layer::LAYER_3)) {
        throw NotSupportedStream();
    }

    unsigned char bitRateIndex = reader->nextNBits(4);
    if (bitRateIndex == 0 || bitRateIndex >= 15)
        throw InvalidStreamFormat("bad value of BitRateIndex field");
    header.bitrate = bitRateList[bitRateIndex - 1] * 1000;

    std::size_t freqIndex = reader->nextNBits(2);
    if (freqIndex > 2)
        throw InvalidStreamFormat("bad value of frequencyIndex field");
    header.samplingFreq = samplingFreqs[freqIndex];

    header.isPadded = reader->nextBit();
    reader->nextBit();  // Skip private bit

    header.mode = static_cast<FrameHeader::Mode>(reader->nextNBits(2));
    header.msStereo = reader->nextBit();
    header.intensityStereo = reader->nextBit();
    if (header.mode == FrameHeader::Mode::JOINT_STEREO && header.intensityStereo)
        throw InvalidStreamFormat("Joint stereo files using intensity stereo are not supported");

    reader->nextNBits(4);  // Emphasis field is omitted

    bytesInHeaders = 4;
}

void FrameDecoder::skipCRC() {
    if (frame.header.usesCRC) {
        reader->nextByte();
        reader->nextByte();
        bytesInHeaders += 2;
    }
}

void FrameDecoder::decodeSideInformation() {
    const auto channels = frame.header.channels();

    auto& sideInfo = frame.sideInfo;
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
            chSideInfo.globalGain = static_cast<float>(reader->nextNBits(8));
            chSideInfo.scaleFactorCompression = reader->nextNBits(4);
            chSideInfo.windowSwitching = reader->nextBit();

            if (chSideInfo.windowSwitching) {
                chSideInfo.blockType = static_cast<GranuleChannelSideInfo::BlockType>(reader->nextNBits(2));
                chSideInfo.mixedBlockFlag = reader->nextBit();
                for (std::size_t i = 0; i < REGIONS_WINDOW_SWITCHING; i++) {
                    chSideInfo.tableSelect[i] = reader->nextNBits(5);
                }

                for (std::size_t i = 0; i < NR_SHORT_WINDOWS; i++) {
                    chSideInfo.subBlockGain[i] = reader->nextNBits(3);
                }
                setRegionCountForGranule(chSideInfo);
            } else {
                chSideInfo.blockType = GranuleChannelSideInfo::BlockType::NORMAL;
                for (std::size_t i = 0; i < REGIONS_NORMAL_BLOCK; i++) {
                    chSideInfo.tableSelect[i] = reader->nextNBits(5);
                }

                chSideInfo.region0_count = reader->nextNBits(4);
                chSideInfo.region1_count = reader->nextNBits(3);
            }

            chSideInfo.preFlag = static_cast<float>(reader->nextBit());
            chSideInfo.scaleFactorScale = static_cast<float>(reader->nextBit());
            chSideInfo.count1TableSelect = reader->nextBit();
        }
    }

    auto sideInfoSize = (channels == 1) ? SIDE_INFO_SIZE_MONO : SIDE_INFO_SIZE_DUAL;
    bytesInHeaders += sideInfoSize;
}

void FrameDecoder::setRegionCountForGranule(GranuleChannelSideInfo& chGranule) {
    if (chGranule.blockType == GranuleChannelSideInfo::BlockType::NORMAL)
        throw InvalidStreamFormat(
            "value \"NORMAL\" for blockType is not allowed when windowSwitching flag is set");

    if (chGranule.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT && !chGranule.mixedBlockFlag) {
        chGranule.region0_count = 8;
    } else {
        chGranule.region0_count = 7;
    }

    chGranule.region1_count = 20 - chGranule.region0_count;
}

void FrameDecoder::decodeMainData() {
    reader->startFrame(frame.sideInfo.mainDataBegin);

    const auto channels = frame.header.channels();
    bool readingSecondGranule = false;
    for (unsigned int i = 0; i < NR_GRANULES; i++) {
        unsigned int channel;
        for (channel = 0; channel < channels; channel++) {
            const auto& channelInfo = frame.sideInfo.granules[i][channel];
            auto& channelContent = frame.content.granules[i][channel];
            auto channelStart = reader->bitsRead();
            readChannelScaleFactors(channelInfo, channelContent, channel, readingSecondGranule);
            entropyDecode(channelInfo, channelStart, channelContent);
        }

        readingSecondGranule = true;
    }

    reader->frameEnded(frame.frameSize, bytesInHeaders);
}

void FrameDecoder::readChannelScaleFactors(const GranuleChannelSideInfo& channelSideInfo,
                                           GranuleChannelContent& channelContent,
                                           unsigned int channel,
                                           bool readingSecondGranule) {
    const unsigned char slen1 = scaleFactorsCompression[0][channelSideInfo.scaleFactorCompression];
    const unsigned char slen2 = scaleFactorsCompression[1][channelSideInfo.scaleFactorCompression];

    if (channelSideInfo.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        readShortWindowScaleFactors(channelContent, channelSideInfo, slen1, slen2);
    } else {
        readLongWindowScaleFactors(channelContent, channel, readingSecondGranule, slen1, slen2);
    }
}

void FrameDecoder::readShortWindowScaleFactors(GranuleChannelContent& channelContent,
                                               const GranuleChannelSideInfo& channelSideInfo,
                                               unsigned char slen1,
                                               unsigned char slen2) {
    if (channelSideInfo.mixedBlockFlag) {
        std::size_t band;
        for (band = 0; band < 8; band++) {
            channelContent.longWindowScaleFactorBands[band] = reader->nextNBits(slen1);
        }
        for (band = 3; band < NR_SHORT_WINDOW_BANDS; band++) {
            unsigned char toRead = (band < 6) ? slen1 : slen2;
            for (std::size_t window = 0; window < NR_SHORT_WINDOWS; window++) {
                channelContent.shortWindowScaleFactorBands[window][band] = reader->nextNBits(toRead);
            }
        }
    } else {
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
        for (unsigned int window = 0; window < NR_SHORT_WINDOWS; window++) {
            channelContent.shortWindowScaleFactorBands[window][NR_SHORT_WINDOW_BANDS] = 0;
        }
    }
}

void FrameDecoder::readLongWindowScaleFactors(GranuleChannelContent& channelContent,
                                              unsigned int channel,
                                              bool readingSecondGranule,
                                              unsigned char slen1,
                                              unsigned char slen2) {
    for (unsigned int group = 0; group < NR_SUB_BAND_GROUPS; group++) {
        bool scaleFactorsAreShared =
            readingSecondGranule && frame.sideInfo.scaleFactorSharing[channel][group];
        const unsigned int subBandStart = scaleFactorBandsGroups[1][group];
        const unsigned int subBandEnd = scaleFactorBandsGroups[1][group + 1];
        if (!scaleFactorsAreShared) {
            unsigned char toRead = (group < 2) ? slen1 : slen2;
            for (unsigned int i = subBandStart; i < subBandEnd; i++) {
                channelContent.longWindowScaleFactorBands[i] = reader->nextNBits(toRead);
            }
        } else {
            const auto& firstGranuleSf = frame.content.granules[0][channel].longWindowScaleFactorBands;
            for (unsigned int i = subBandStart; i < subBandEnd; i++) {
                channelContent.longWindowScaleFactorBands[i] = firstGranuleSf[i];
            }
        }
    }
}

void FrameDecoder::entropyDecode(const GranuleChannelSideInfo& channelInfo,
                                 unsigned long channelStart,
                                 GranuleChannelContent& content) {
    unsigned int region0Samples;
    unsigned int region1Samples;

    if (channelInfo.windowSwitching &&
        channelInfo.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        region0Samples = 36; /* sfb[9/3]*3=36 */
        region1Samples = NR_GRANULE_SAMPLES;
    } else {
        const auto& indexesTable = samplingFreqBandIndexes[frame.header.samplingFreq].longWindow;
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
        storeInContent(x, freqLinesDecoded, content.freqBands);
        storeInContent(y, freqLinesDecoded + 1, content.freqBands);
        freqLinesDecoded += 2;
    }

    const auto& count1Table = huffmanSet->getTable(32 + channelInfo.count1TableSelect);
    while (reader->bitsRead() < channelStart + channelInfo.part2_3_length) {
        int x, y, v, w;
        count1Table.decodeCount1(*reader, x, y, v, w);
        storeInContent(v, freqLinesDecoded, content.freqBands);
        storeInContent(w, freqLinesDecoded + 1, content.freqBands);
        storeInContent(x, freqLinesDecoded + 2, content.freqBands);
        storeInContent(y, freqLinesDecoded + 3, content.freqBands);
        freqLinesDecoded += 4;
    }

    for (; freqLinesDecoded < NR_GRANULE_SAMPLES; freqLinesDecoded++) {
        storeInContent(0, freqLinesDecoded, content.freqBands);
    }
}
