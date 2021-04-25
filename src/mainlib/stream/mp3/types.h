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
constexpr std::size_t NR_SHORT_WINDOW_RANGES = NR_SHORT_WINDOW_BANDS + 1;
constexpr std::size_t NR_SHORT_WINDOW_BAND_SAMPLES = NR_SHORT_WINDOW_BANDS / 2;
constexpr std::size_t NR_FREQ_BANDS = 32;              // Used in frequency-domain operations
constexpr std::size_t NR_CODED_SAMPLES_PER_BAND = 18;  // Used in frequency-domain operations
constexpr std::size_t NR_TOTAL_SAMPLES = 2 * NR_CODED_SAMPLES_PER_BAND;
constexpr std::size_t NR_PCM_BLOCKS = 18;     // Used in time-domain operations
constexpr std::size_t NR_BLOCK_SAMPLES = 32;  // Used in time-domain operations
constexpr std::size_t NR_GRANULE_SAMPLES = NR_PCM_BLOCKS * NR_BLOCK_SAMPLES;
constexpr std::size_t NR_FRAME_SAMPLES = NR_GRANULE_SAMPLES * NR_GRANULES;

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
    unsigned short scaleFactorCompression;
    bool windowSwitching;
    BlockType blockType;
    bool mixedBlockFlag;
    std::array<unsigned char, NR_REGIONS> tableSelect;
    std::array<unsigned char, NR_SHORT_WINDOWS> subBlockGain;
    unsigned char region0_count;
    unsigned char region1_count;
    float preFlag;
    float scaleFactorScale;
    unsigned char count1TableSelect;
};

template <class Content>
using GranuleChannelsData = std::array<std::array<Content, NR_CHANNELS>, NR_GRANULES>;

struct SideInformation {
    unsigned short mainDataBegin;
    std::array<std::array<bool, NR_SUB_BAND_GROUPS>, NR_CHANNELS> scaleFactorSharing;
    GranuleChannelsData<GranuleChannelSideInfo> granules;
};

using ShortWindowScaleFactors = std::array<std::array<int, NR_SHORT_WINDOW_BANDS>, NR_SHORT_WINDOWS>;

template <class Representation>
using Bands = std::array<std::array<Representation, NR_CODED_SAMPLES_PER_BAND>, NR_FREQ_BANDS>;

struct GranuleChannelContent {
    std::array<int, NR_LONG_WINDOW_BANDS> longWindowScaleFactorBands;
    ShortWindowScaleFactors shortWindowScaleFactorBands;
    Bands<int> freqBands;
};

struct MainDataContent {
    GranuleChannelsData<GranuleChannelContent> granules;
};

struct Frame {
    unsigned int frameSize;
    FrameHeader header;
    SideInformation sideInfo;
    MainDataContent content;
};

using ChannelSamples = std::array<short, NR_FRAME_SAMPLES>;
struct FrameSamples {
    ChannelSamples channel1;
    ChannelSamples channel2;
};

using FrameStartToken = unsigned char;

extern std::map<std::size_t, BandIndexes> samplingFreqBandIndexes;

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_TYPES_H
