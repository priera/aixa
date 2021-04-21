#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H

#include "BlockSynthesisAlgorithms.h"

class ShortWindowAlgorithms : public BlockSynthesisAlgorithms {
public:
    Bands<double> computeScaleFactors(unsigned int samplingFreq,
                                      const GranuleChannelSideInfo& channelInfo,
                                      const GranuleChannelContent& channelContent) override;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H
