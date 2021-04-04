#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_FRAMEDECODER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_FRAMEDECODER_H

#include <mainlib/stream/mp3/huffman/HuffmanSet.h>
#include <mainlib/stream/mp3/streamread/MainDataReader.h>
#include <mainlib/stream/mp3/types.h>

class FrameDecoder {
public:
    FrameDecoder(MainDataReader& reader, std::unique_ptr<HuffmanSet> huffmanSet) :
        reader(&reader), huffmanSet(std::move(huffmanSet)), bytesInHeaders(0), frame() {}

    virtual ~FrameDecoder() noexcept = default;

    const Frame& decode(FrameStartToken token);

private:
    static constexpr std::size_t REGIONS_NORMAL_BLOCK = 3;
    static constexpr std::size_t REGIONS_WINDOW_SWITCHING = 2;

    static constexpr unsigned int SIDE_INFO_SIZE_MONO = 17;
    static constexpr unsigned int SIDE_INFO_SIZE_DUAL = 32;

    static std::vector<unsigned int> bitRateList;
    static std::vector<unsigned int> samplingFreqs;
    static std::vector<std::vector<unsigned int>> scaleFactorBandsGroups;
    static std::vector<std::vector<unsigned char>> scaleFactorsCompression;

    static inline void storeInContent(int sample, unsigned int sampleIndex, Bands<int>& bands) {
        bands[sampleIndex / NR_CODED_SAMPLES_PER_BAND][sampleIndex % NR_CODED_SAMPLES_PER_BAND] = sample;
    }

    void decodeHeader(FrameStartToken tok);
    void computeFrameSize();
    void skipCRC();
    void decodeSideInformation();
    void setRegionCountForGranule(GranuleChannelSideInfo& chGranule);
    void decodeMainData();
    void readChannelScaleFactors(const GranuleChannelSideInfo& channelSideInfo,
                                 GranuleChannelContent& channelContent,
                                 unsigned int channel,
                                 bool readingSecondGranule);
    void entropyDecode(const GranuleChannelSideInfo& channelInfo,
                       unsigned long channelStart,
                       GranuleChannelContent& content);

    MainDataReader* reader;
    std::unique_ptr<HuffmanSet> huffmanSet;

    unsigned int bytesInHeaders;
    Frame frame;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_FRAMEDECODER_H
