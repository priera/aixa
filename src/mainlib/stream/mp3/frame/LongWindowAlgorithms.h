#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWALGORITHMS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWALGORITHMS_H

#include "BlockSynthesisAlgorithms.h"

class LongWindowAlgorithms : public BlockSynthesisAlgorithms {
public:
    Bands<double> computeScaleFactors(unsigned int samplingFreq,
                                      const GranuleChannelSideInfo& channelInfo,
                                      const GranuleChannelContent& channelContent) override;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWALGORITHMS_H
