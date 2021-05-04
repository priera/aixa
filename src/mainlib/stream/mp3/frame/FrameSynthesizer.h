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
    static constexpr float GAIN_BASE = 210.f;
    static constexpr double SCALE = 1 << 15;
    static constexpr std::size_t D_WINDOW_VECTORS = 16;
    static constexpr double MAX_DECODED_VALUE = 32768.0;

    void synthesizeGranuleChannel(ChannelSamples& samples,
                                  unsigned int channel,
                                  const FrameHeader& header,
                                  const GranuleChannelSideInfo& channelInfo,
                                  const GranuleChannelContent& channelContent,
                                  std::size_t startIndex);
    void dequantizeSamples(unsigned int samplingFreq,
                           const GranuleChannelSideInfo& channelInfo,
                           const GranuleChannelContent& channelContent);
    void reorder(unsigned int samplingFreq,
                 const GranuleChannelSideInfo& channelInfo,
                 const GranuleChannelContent& channelContent);
    void stereo(FrameHeader::Mode mode,
                const GranuleChannelSideInfo& channelInfo,
                const GranuleChannelContent& channelContent);
    void antialias(const GranuleChannelSideInfo& channelInfo);
    void inverseMDCT(const GranuleChannelSideInfo& info, Bands<double>& overlappingTerms);
    void polyphaseSynthesis(ChannelSamples& samples, std::size_t startIndex);

    void resetFIFO();

    std::unique_ptr<BlockSynthesisAlgorithms> longWindowAlgorithms;
    std::unique_ptr<BlockSynthesisAlgorithms> shortWindowAlgorithms;

    AntialiasCoefficients antialiasCoefficients;
    BlockWindows blockWindows;
    aixa::math::DoubleMatrix frequencyInversion;
    aixa::math::DoubleMatrix synthesisFilter;
    const aixa::math::DoubleMatrix dWindow;

    aixa::math::DoubleMatrix dequantized;
    aixa::math::DoubleMatrix timeSamples;

    std::array<Bands<double>, NR_CHANNELS> channelOverlappingTerms;

    std::deque<aixa::math::DoubleVector> fifo;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZER_H
