#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H

#include "BlockSynthesisAlgorithms.h"

class ShortWindowAlgorithms : public BlockSynthesisAlgorithms {
public:
    Bands<double> computeScaleFactors(unsigned int samplingFreq,
                                      const GranuleChannelSideInfo& channelInfo,
                                      const GranuleChannelContent& channelContent) override;

    aixa::math::DoubleMatrix computeInverseMDCT(const aixa::math::DoubleMatrix& dequantized,
                                                const aixa::math::DoubleMatrix& window) override;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H
