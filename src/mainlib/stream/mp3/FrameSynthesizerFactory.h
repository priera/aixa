#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZERFACTORY_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZERFACTORY_H

#include "FrameSynthesizer.h"

class FrameSynthesizerFactory {
public:
    virtual ~FrameSynthesizerFactory() = default;

    FrameSynthesizer *build() const;

private:
    FrameSynthesizer::AntialiasCoefficients computeAntialiasCoefficients() const;
    aixa::math::DoubleMatrix computeTransformMatrix() const;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_FRAMESYNTHESIZERFACTORY_H