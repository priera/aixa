#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_BLOCKSYNTHESISALGORITHMS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_BLOCKSYNTHESISALGORITHMS_H

#include <mainlib/math/types.h>
#include <mainlib/stream/mp3/types.h>

class BlockSynthesisAlgorithms {
public:
    virtual ~BlockSynthesisAlgorithms() noexcept = default;

    virtual Bands<double> computeScaleFactors(unsigned int samplingFreq,
                                              const GranuleChannelSideInfo& channelInfo,
                                              const GranuleChannelContent& channelContent) = 0;

    virtual aixa::math::DoubleMatrix computeInverseMDCT(const aixa::math::DoubleMatrix& dequantized,
                                                        const aixa::math::DoubleMatrix& window) = 0;

protected:
    static constexpr std::array<unsigned int, 22> pretab = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                            1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0};
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAME_BLOCKSYNTHESISALGORITHMS_H
