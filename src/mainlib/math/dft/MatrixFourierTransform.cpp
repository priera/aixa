#include "MatrixFourierTransform.h"
#include "mainlib/math/ComplexExponential.h"

#include <cmath>

using namespace aixa::math;

const ComplexVector &MatrixFourierTransform::transform(const ComplexVector &complexSignal) {
    return basis * complexSignal;
}
