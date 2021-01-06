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

    bool isMono() const { return mode == Mode::SINGLE_CHANNEL; }
};

constexpr std::size_t NR_CHANNELS = 2;
constexpr std::size_t NR_GRANULES = 2;
constexpr std::size_t NR_SCALEBANDS = 4;

struct GranuleSideInformation {
    unsigned short part2_3_length[NR_CHANNELS];
    unsigned short bigValues[NR_CHANNELS];
    char globalGain[NR_CHANNELS];
    unsigned char scaleFactorCompression[NR_CHANNELS];
    bool windowSwitching[NR_CHANNELS];
};

struct SideInformation {
    unsigned short mainDataBegin;
    bool scaleFactorSharing[NR_CHANNELS][NR_SCALEBANDS];
    GranuleSideInformation granules[NR_GRANULES];
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMEHEADER_H
