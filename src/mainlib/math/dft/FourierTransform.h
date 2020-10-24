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

        double baseDiscreteFreq() const {
            return (2 * M_PI) / N;
        }

        double baseContinuousFreq(double samplePeriod) const {
            return 1 / (N * samplePeriod);
        }

        unsigned int dimensionality() const {
            return N;
        }

    protected:
        virtual const ComplexVector &transform(const ComplexVector &complexSignal) = 0;

        unsigned int N;
    };
}

#endif //AIXA_SRC_MAINLIB_MATH_FOURIERTRANSFORM_H
