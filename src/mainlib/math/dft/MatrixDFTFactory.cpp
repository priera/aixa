#include "MatrixDFTFactory.h"
#include "MatrixFourierTransform.h"

#include <cmath>

#include <mainlib/math/ComplexExponential.h>

namespace aixa::math {
    FourierTransform* MatrixDFTFactory::build(unsigned int dimensionality) {
        auto basisVectors = dimensionality;

        auto fourierBasis = ComplexBasis(dimensionality, basisVectors);

        double fundamentalFrequency = (2 * M_PI) / dimensionality;

        for (size_t k = 0; k < basisVectors; k++) {
            for (size_t n = 0; n < dimensionality; n++) {
                auto ex = ~(ComplexExponential(fundamentalFrequency * n * k));
                fourierBasis(k, n) = Complex(ex.re(), ex.im());
            }
        }

        return new MatrixFourierTransform(dimensionality, fourierBasis);
    }
}
