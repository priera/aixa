#include "FourierTransform.h"
#include "ComplexExponential.h"

#include <cmath>

using namespace aixa::math;

FourierTransform FourierTransform::prepare(size_t dimensionality) {
    auto basisVectors = dimensionality;

    auto fourierBasis = ComplexBasis(dimensionality, basisVectors);

    double fundamentalFrequency = (2 * M_PI) / dimensionality;

    for (size_t k = 0; k < basisVectors; k++) {
        for (size_t n = 0; n < dimensionality; n++) {
            auto ex = ~(ComplexExponential(fundamentalFrequency * n * k));
            fourierBasis(k, n) = Complex(ex.re(), ex.im());
        }
    }

    return std::move(FourierTransform(dimensionality, fourierBasis));
}

const ComplexVector &FourierTransform::applyTo(const DoubleVector &signal) {
    auto complexSignal = fromRealVector(signal);
    return basis * complexSignal;
}
