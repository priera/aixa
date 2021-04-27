#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H

#include "BlockSynthesisAlgorithms.h"

class ShortWindowAlgorithms : public BlockSynthesisAlgorithms {
public:
    explicit ShortWindowAlgorithms(aixa::math::DoubleMatrix cosineTransform) :
        cosineTransform(std::move(cosineTransform)) {}

    Bands<double> computeScaleFactors(unsigned int samplingFreq,
                                      const GranuleChannelSideInfo& channelInfo,
                                      const GranuleChannelContent& channelContent) override;

    aixa::math::DoubleMatrix computeInverseMDCT(const aixa::math::DoubleMatrix& dequantized,
                                                const aixa::math::DoubleMatrix& window) override;

private:
    aixa::math::DoubleMatrix cosineTransform;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_SHORTWINDOWALGORITHMS_H
