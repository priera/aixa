#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_TYPES_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_TYPES_H

#include <cstddef>
#include <map>
#include <vector>

struct BandIndexes {
    std::vector<unsigned int> longWindow;
    std::vector<unsigned int> shortWindow;
};

struct FrameHeader {
    enum class Version
    {
        MPEG_1,
        MPEG_2
    };

    enum class Layer : unsigned char
    {
        RESERVED = 0,
        LAYER_3,
        LAYER_2,
        LAYER_1
    };

    enum class Mode : unsigned char
    {
        STEREO,
        JOINT_STEREO,
        DUAL_CHANNEL,
        SINGLE_CHANNEL
    };

    Version version;
    Layer layer;
    bool usesCRC;
    unsigned int bitrate;
    unsigned int samplingFreq;
    bool isPadded;
    Mode mode;

    // Only used in JOINT_STEREO mode
    bool msStereo;
    bool intensityStereo;

    std::size_t channels() const { return (mode == Mode::SINGLE_CHANNEL) ? 1 : 2; }
};

constexpr std::size_t NR_CHANNELS = 2;
constexpr std::size_t NR_GRANULES = 2;
constexpr std::size_t NR_REGIONS = 3;
constexpr std::size_t NR_SHORT_WINDOWS = 3;
constexpr std::size_t NR_SUB_BAND_GROUPS = 4;
constexpr std::size_t NR_LONG_WINDOW_BANDS = 21;
constexpr std::size_t NR_SHORT_WINDOW_BANDS = 12;
constexpr std::size_t SAMPLES_PER_GRANULE = 576;
constexpr std::size_t NR_FREQ_BANDS = 32;
constexpr std::size_t NR_SAMPLES_PER_BAND = 18;

static constexpr unsigned int SAMPLES_PER_FRAME = SAMPLES_PER_GRANULE * 2;

struct GranuleChannelSideInfo {
    enum class BlockType : unsigned char
    {
        NORMAL = 0,
        START,
        THREE_SHORT,
        END
    };

    unsigned short part2_3_length;
    unsigned short bigValues;
    float globalGain;
    unsigned char scaleFactorCompression;
    bool windowSwitching;
    BlockType blockType;
    bool mixedBlockFlag;
    unsigned char tableSelect[NR_REGIONS];
    unsigned char subBlockGain[NR_SHORT_WINDOWS];
    unsigned char region0_count;
    unsigned char region1_count;
    float preFlag;
    float scaleFactorScale;
    unsigned char count1TableSelect;
};

struct SideInformation {
    unsigned short mainDataBegin;
    bool scaleFactorSharing[NR_CHANNELS][NR_SUB_BAND_GROUPS];
    GranuleChannelSideInfo granules[NR_GRANULES][NR_CHANNELS];
};

using ShortWindowScaleFactors = std::vector<std::vector<int>>;
using FrequencyBands = std::vector<std::vector<int>>;

struct GranuleChannelContent {
    GranuleChannelContent() :
        longWindowScaleFactorBands(NR_LONG_WINDOW_BANDS), shortWindowScaleFactorBands(NR_SHORT_WINDOWS),
        freqBands(NR_FREQ_BANDS) {
        for (auto& shortWindow : shortWindowScaleFactorBands) {
            shortWindow.resize(NR_SHORT_WINDOW_BANDS);
        }
        for (auto& samplesVector : freqBands) {
            samplesVector.resize(NR_SAMPLES_PER_BAND);
        }
    }

    std::vector<int> longWindowScaleFactorBands;
    ShortWindowScaleFactors shortWindowScaleFactorBands;
    FrequencyBands freqBands;
};

struct MainDataContent {
    MainDataContent() : granules(NR_GRANULES) {
        for (auto& granule : granules) {
            granule.resize(NR_CHANNELS);
        }
    }

    std::vector<std::vector<GranuleChannelContent>> granules;
};

extern std::map<int, BandIndexes> samplingFreqBandIndexes;

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_TYPES_H
