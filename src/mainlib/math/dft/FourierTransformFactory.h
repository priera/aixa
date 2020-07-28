#ifndef AIXA_SRC_MAINLIB_MATH_DFT_FOURIERTRANSFORMFACTORY_H
#define AIXA_SRC_MAINLIB_MATH_DFT_FOURIERTRANSFORMFACTORY_H

#include "FourierTransform.h"

namespace aixa::math {

    class FourierTransformFactory {
    public:
        enum class Implementations {
            MATRIX,
            FFT
        };

        virtual ~FourierTransformFactory() = default;

        virtual FourierTransform *build(unsigned int dimensionality) = 0;
    };

    FourierTransformFactory& getFourierTransformFactory(FourierTransformFactory::Implementations impl);
}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_FOURIERTRANSFORMFACTORY_H
