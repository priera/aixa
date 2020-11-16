#ifndef AIXA_SRC_MAINLIB_MATH_FOURIERTRANSFORM_H
#define AIXA_SRC_MAINLIB_MATH_FOURIERTRANSFORM_H

#include <cmath>

#include "mainlib/math/types.h"

namespace aixa::math {
    class FourierTransform {
    public:
        explicit FourierTransform(unsigned int N) :
            N(N) {};

        virtual ~FourierTransform() = default;

        const ComplexVector &applyTo(const DoubleVector &signal);

        constexpr double baseDiscreteFreq() const {
            return (2 * M_PI) / N;
        }

        constexpr double baseContinuousFreq(double samplePeriod) const {
            return 1 / (N * samplePeriod);
        }

        constexpr double maxContinuousFreq(double samplePeriod) const {
            return 1 / (2 * samplePeriod);
        }

        constexpr std::size_t relevantSize() const {
            return (N / 2) + 1;
        }

        constexpr unsigned int dimensionality() const {
            return N;
        }

    protected:
        virtual const ComplexVector &transform(const ComplexVector &complexSignal) = 0;

        unsigned int N;
    };
}

#endif //AIXA_SRC_MAINLIB_MATH_FOURIERTRANSFORM_H
