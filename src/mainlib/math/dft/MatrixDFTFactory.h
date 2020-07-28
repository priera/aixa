#ifndef AIXA_SRC_MAINLIB_MATH_DFT_MATRIXDFTFACTORY_H
#define AIXA_SRC_MAINLIB_MATH_DFT_MATRIXDFTFACTORY_H

#include "FourierTransformFactory.h"

namespace aixa::math {
    class MatrixDFTFactory : public FourierTransformFactory {
    public:

        FourierTransform *build(unsigned int dimensionality) override;
    };
}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_MATRIXDFTFACTORY_H
