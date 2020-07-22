#ifndef AIXA_SRC_MAINLIB_DSP_FOURIERTRANSFORM_H
#define AIXA_SRC_MAINLIB_DSP_FOURIERTRANSFORM_H

#include <cmath>

#include "Matrix.h"
#include "Vector.h"
#include "types.h"

namespace aixa::math {
    class FourierTransform {
    public:
        static FourierTransform prepare(size_t dimensionality);

        virtual ~FourierTransform() = default;

        const ComplexVector &applyTo(const DoubleVector &signal);

    private:
        FourierTransform(size_t dimensionality, ComplexBasis basis) :
                dimensionality(dimensionality),
                basis(std::move(basis)) {}

        FourierTransform(FourierTransform &&other) noexcept = default;

        double baseDiscreteFreq() const {
            return (2 * M_PI) / dimensionality;
        }

        double baseContinuousFreq(double samplePeriod) const {
            return 1 / (dimensionality * samplePeriod);
        }

        size_t dimensionality;
        ComplexBasis basis;

#ifdef QTEST
        friend class aixa::math::FourierTransform_UTest;
#endif
    };
}

#endif //AIXA_SRC_MAINLIB_DSP_FOURIERTRANSFORM_H
