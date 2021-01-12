#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMEHEADER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMEHEADER_H

#include <cstddef>

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
    unsigned char globalGain;
    unsigned char scaleFactorCompression;
    bool windowSwitching;
    BlockType blockType;
    bool mixedBlockFlag;
    unsigned char tableSelect[NR_REGIONS];
    unsigned char subBlockGain[NR_SHORT_WINDOWS];
    unsigned char region0_count;
    unsigned char region1_count;
    bool preFlag;
    bool scaleFactorScale;
    bool count1TableSelect;
};

struct SideInformation {
    unsigned short mainDataBegin;
    bool scaleFactorSharing[NR_CHANNELS][NR_SUB_BAND_GROUPS];
    GranuleChannelSideInfo granules[NR_GRANULES][NR_CHANNELS];
};

struct GranuleChannelContent {
    int longWindowScaleFactorBands[NR_LONG_WINDOW_BANDS];
    int shortWindowScaleFactorBands[NR_SHORT_WINDOWS][NR_SHORT_WINDOW_BANDS];
};

struct MainDataContent {
    GranuleChannelContent granules[NR_GRANULES][NR_CHANNELS];
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMEHEADER_H
