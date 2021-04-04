#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWSCALEFACTORSCOMPUTER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWSCALEFACTORSCOMPUTER_H

#include "WindowScaleFactorsComputer.h"

class ShortWindowScaleFactorsComputer : public WindowScaleFactorsComputer {
public:
    Bands<double> compute(unsigned int samplingFreq,
                          const GranuleChannelSideInfo& channelInfo,
                          const GranuleChannelContent& channelContent) override;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWSCALEFACTORSCOMPUTER_H
