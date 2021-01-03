#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMEHEADER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMEHEADER_H

struct FrameHeader {
    enum class Version { MPEG_1, MPEG_2 };

    enum class Layer : unsigned char { RESERVED = 0, LAYER_3, LAYER_2, LAYER_1 };

    enum class Mode : unsigned char { STEREO, JOINT_STEREO, DUAL_CHANNEL, SINGLE_CHANNEL };

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
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMEHEADER_H
