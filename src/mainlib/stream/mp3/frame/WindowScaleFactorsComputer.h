#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_WINDOWSCALEFACTORSCOMPUTER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_WINDOWSCALEFACTORSCOMPUTER_H

#include <mainlib/stream/mp3/types.h>

class WindowScaleFactorsComputer {
public:
    virtual ~WindowScaleFactorsComputer() noexcept = default;

    virtual Bands<double> compute(unsigned int samplingFreq,
                                  const GranuleChannelSideInfo& channelInfo,
                                  const GranuleChannelContent& channelContent) = 0;

protected:
    static constexpr std::array<unsigned int, 22> pretab = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                            1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0};
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_WINDOWSCALEFACTORSCOMPUTER_H
