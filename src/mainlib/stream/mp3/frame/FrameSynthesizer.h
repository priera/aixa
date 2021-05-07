#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H

#include <mainlib/math/types.h>

#include <deque>

#include "BlockSynthesisAlgorithms.h"
#include "mainlib/stream/mp3/types.h"

class FrameSynthesizer {
public:
    static constexpr std::size_t NR_BUTTERFLIES = 8;
    static constexpr std::size_t NR_FIFO_SIZE = 16;
    static constexpr std::size_t NR_D_WINDOW_VECTOR_SIZE = 32;
    static constexpr std::size_t NR_D_WINDOW_MATRIXED_VECTOR_SIZE = NR_D_WINDOW_VECTOR_SIZE * 2;

    struct AntialiasCoefficients {
        std::array<double, NR_BUTTERFLIES> ca;
        std::array<double, NR_BUTTERFLIES> cs;
    };

    using BlockWindows = std::map<GranuleChannelSideInfo::BlockType, aixa::math::DoubleMatrix>;

    FrameSynthesizer(std::unique_ptr<BlockSynthesisAlgorithms> longWindowSFComputer,
                     std::unique_ptr<BlockSynthesisAlgorithms> shortWindowSFComputer,
                     AntialiasCoefficients antialiasCoefficients,
                     BlockWindows blockWindows,
                     aixa::math::DoubleMatrix frequencyInversion,
                     aixa::math::DoubleMatrix synFilter,
                     aixa::math::Matrix<double, aixa::math::DoubleTypeAxioms> matrix);
    virtual ~FrameSynthesizer() = default;

    FrameSamples synthesize(const Frame& frame);
    void clearState();

private:
    using ChannelsDequantized = std::array<aixa::math::DoubleMatrix, NR_CHANNELS>;
    using GranulesDequantized = std::array<ChannelsDequantized, NR_GRANULES>;
    using ChannelsOverlapping = std::array<Bands<double>, NR_CHANNELS>;
    using ChannelFifo = std::deque<aixa::math::DoubleVector>;

    static GranulesDequantized buildGranulesDequantized();

    static constexpr float GAIN_BASE = 210.f;
    static constexpr double SCALE = 1 << 15;
    static constexpr std::size_t D_WINDOW_VECTORS = 16;
    static constexpr double MAX_DECODED_VALUE = 32768.0;

    aixa::math::DoubleMatrix dequantizeSamples(unsigned int samplingFreq,
                                               const GranuleChannelSideInfo& channelInfo,
                                               const GranuleChannelContent& channelContent);
    void jointStereo(const FrameHeader& header, ChannelsDequantized& channelsDequantized);

    void synthesizeGranuleChannel(ChannelSamples& samples,
                                  aixa::math::DoubleMatrix& dequantized,
                                  Bands<double>& overlapping,
                                  ChannelFifo& fifo,
                                  const FrameHeader& header,
                                  const GranuleChannelSideInfo& channelInfo,
                                  std::size_t startIndex);

    void reorder(aixa::math::DoubleMatrix& dequantized,
                 unsigned int samplingFreq,
                 const GranuleChannelSideInfo& channelInfo);

    void antialias(aixa::math::DoubleMatrix& dequantized, const GranuleChannelSideInfo& channelInfo);

    void inverseMDCT(const aixa::math::DoubleMatrix& dequantized,
                     const GranuleChannelSideInfo& info,
                     Bands<double>& overlappingTerms);

    void polyphaseSynthesis(ChannelSamples& samples, ChannelFifo& fifo, std::size_t startIndex);

    void resetFIFOs();

    std::unique_ptr<BlockSynthesisAlgorithms> longWindowAlgorithms;
    std::unique_ptr<BlockSynthesisAlgorithms> shortWindowAlgorithms;

    AntialiasCoefficients antialiasCoefficients;
    BlockWindows blockWindows;
    aixa::math::DoubleMatrix frequencyInversion;
    aixa::math::DoubleMatrix synthesisFilter;
    const aixa::math::DoubleMatrix dWindow;

    GranulesDequantized granulesDequantized;

    aixa::math::DoubleMatrix timeSamples;

    ChannelsOverlapping overlappingTerms;

    std::array<ChannelFifo, NR_CHANNELS> fifoOfChannel;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
