#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWSCALEFACTORSCOMPUTER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWSCALEFACTORSCOMPUTER_H

#include "WindowScaleFactorsComputer.h"

class LongWindowScaleFactorsComputer : public WindowScaleFactorsComputer {
public:
    Bands<double> compute(unsigned int samplingFreq,
                          const GranuleChannelSideInfo& channelInfo,
                          const GranuleChannelContent& channelContent) override;

private:
    static std::vector<unsigned int> pretab;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWSCALEFACTORSCOMPUTER_H
