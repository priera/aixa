#ifndef AIXA_SRC_MAINLIB_MATH_DFT_FOURIERTRANSFORMFACTORY_H
#define AIXA_SRC_MAINLIB_MATH_DFT_FOURIERTRANSFORMFACTORY_H

#include <platform/aixa_export.h>

#include "FourierTransform.h"

namespace aixa::math {

    class LIB_EXPORT FourierTransformFactory {
    public:
        enum class Implementations {
            MATRIX,
            FFT
        };

        virtual ~FourierTransformFactory() = default;

        virtual FourierTransform *build(unsigned int dimensionality) = 0;
    };

    FourierTransformFactory& LIB_EXPORT getFourierTransformFactory(FourierTransformFactory::Implementations impl);
}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_FOURIERTRANSFORMFACTORY_H
