#ifndef AIXA_SRC_MAINLIB_MATH_FOURIERTRANSFORM_H
#define AIXA_SRC_MAINLIB_MATH_FOURIERTRANSFORM_H

#include <cmath>

#include <platform/aixa_export.h>
#include "mainlib/math/types.h"

namespace aixa::math {
class LIB_EXPORT FourierTransform {
public:
    explicit FourierTransform(unsigned int N) : N(N){};

    virtual ~FourierTransform() = default;

    const ComplexVector &applyTo(const DoubleVector &signal);

    double baseDiscreteFreq() const { return (2 * M_PI) / N; }

    double baseContinuousFreq(double samplePeriod) const { return 1 / (N * samplePeriod); }

    double maxContinuousFreq(double samplePeriod) const { return 1 / (2 * samplePeriod); }

    std::size_t relevantSize() const { return (N / 2) + 1; }

    unsigned int dimensionality() const { return N; }

protected:
    virtual const ComplexVector &transform(const ComplexVector &complexSignal) = 0;

    unsigned int N;
};
}  // namespace aixa::math

#endif  // AIXA_SRC_MAINLIB_MATH_FOURIERTRANSFORM_H
