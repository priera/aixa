#ifndef AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMBUILDER_H
#define AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMBUILDER_H

#include "FourierTransformFactory.h"
#include "SpectrogramComputer.h"

namespace aixa::math {
class SpectrogramBuilder {
public:
    explicit SpectrogramBuilder(double samplePeriod) :
        samplePeriod(samplePeriod),
        fourierImplementation(FourierTransformFactory::Implementations::FFT),
        transformSize(2048) {}

    virtual ~SpectrogramBuilder() noexcept = default;

    SpectrogramBuilder &setFourierTransform(FourierTransformFactory::Implementations impl) {
        this->fourierImplementation = impl;
        return *this;
    }

    SpectrogramBuilder &setTransformSize(unsigned int transformSize) {
        this->transformSize = transformSize;
        return *this;
    }

    SpectrogramComputer *build(bool useLogScale = false) const;

private:
    Scale *buildScale(const FourierTransform &fourierTransform, bool useLogScale) const;

    double samplePeriod;
    FourierTransformFactory::Implementations fourierImplementation;
    unsigned int transformSize;
};
}  // namespace aixa::math

#endif  // AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMBUILDER_H
