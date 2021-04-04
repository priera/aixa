#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZERFACTORY_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZERFACTORY_H

#include <filesystem>

#include "FrameSynthesizer.h"

class FrameSynthesizerFactory {
public:
    explicit FrameSynthesizerFactory(std::filesystem::path filePath) : dWindowPath(std::move(filePath)) {}
    virtual ~FrameSynthesizerFactory() = default;

    FrameSynthesizer *build() const;

private:
    FrameSynthesizer::AntialiasCoefficients computeAntialiasCoefficients() const;
    aixa::math::DoubleMatrix computeTransformMatrix() const;
    FrameSynthesizer::BlockWindows generateBlockWindows() const;
    aixa::math::DoubleMatrix computeFrequencyInversionMatrix() const;
    aixa::math::DoubleMatrix computeTimeDomainSynFilter() const;
    aixa::math::DoubleMatrix parseDWindowMatrix() const;

    std::filesystem::path dWindowPath;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZERFACTORY_H
