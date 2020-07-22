#ifndef AIXA_SRC_MAINLIB_MATH_TYPES_H
#define AIXA_SRC_MAINLIB_MATH_TYPES_H

#include "Matrix.h"
#include "Vector.h"
#include "Complex.h"
#include "BasisMatrix.h"

namespace aixa::math {
    using FloatMatrix = Matrix<float>;
    using DoubleMatrix = Matrix<double>;
    using ComplexMatrix = Matrix<Complex>;
    using ComplexBasis = BasisMatrix<Complex>;

    using DoubleVector = Vector<double>;
    using FloatVector = Vector<float>;
    using ComplexVector = Vector<Complex>;

    ComplexVector fromRealVector(const DoubleVector& realVector);
}

#endif //AIXA_SRC_MAINLIB_MATH_TYPES_H
