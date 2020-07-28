#ifndef AIXA_SRC_MAINLIB_MATH_TYPES_H
#define AIXA_SRC_MAINLIB_MATH_TYPES_H

#include "Matrix.h"
#include "Vector.h"
#include "Complex.h"
#include "BasisMatrix.h"
#include "typeAxioms.h"

namespace aixa::math {
    using FloatMatrix = Matrix<float, DoubleTypeAxioms>;
    using DoubleMatrix = Matrix<double, DoubleTypeAxioms>;
    using ComplexMatrix = Matrix<Complex, ComplexTypeAxioms>;
    using ComplexBasis = BasisMatrix<Complex, ComplexTypeAxioms>;

    using DoubleVector = Vector<double, DoubleTypeAxioms>;
    using FloatVector = Vector<float, DoubleTypeAxioms>;
    using ComplexVector = Vector<Complex, ComplexTypeAxioms>;

    ComplexVector fromRealVector(const DoubleVector& realVector);
}

#endif //AIXA_SRC_MAINLIB_MATH_TYPES_H
