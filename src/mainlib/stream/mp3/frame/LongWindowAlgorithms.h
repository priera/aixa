#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWALGORITHMS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWALGORITHMS_H

#include "BlockSynthesisAlgorithms.h"

class LongWindowAlgorithms : public BlockSynthesisAlgorithms {
public:
    explicit LongWindowAlgorithms(aixa::math::DoubleMatrix cosineTransform) :
        cosineTransform(std::move(cosineTransform)) {}

    Bands<double> computeScaleFactors(unsigned int samplingFreq,
                                      const GranuleChannelSideInfo& channelInfo,
                                      const GranuleChannelContent& channelContent) override;

    aixa::math::DoubleMatrix computeInverseMDCT(const aixa::math::DoubleMatrix& dequantized,
                                                const aixa::math::DoubleMatrix& window) override;

private:
    aixa::math::DoubleMatrix cosineTransform;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_LONGWINDOWALGORITHMS_H
