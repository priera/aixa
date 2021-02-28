#ifndef AIXA_SRC_MAINLIB_MATH_BASISMATRIX_H
#define AIXA_SRC_MAINLIB_MATH_BASISMATRIX_H

#include "Matrix.h"
#include "Vector.h"

namespace aixa::math {

template <typename T, class ZeroComparer>
class BasisMatrix : public Matrix<T, ZeroComparer> {
public:
    // N columns, M rows
    BasisMatrix(size_t N, size_t M, T def = T()) :
        Matrix<T, ZeroComparer>(N, M, def), result(std::move(Vector<T, ZeroComparer>(M))){};

    Vector<T, ZeroComparer>& operator*(const Vector<T, ZeroComparer>& other) {
        this->multiply(other, result);
        return result;
    }

private:
    Vector<T, ZeroComparer> result;
};
}  // namespace aixa::math

#endif  // AIXA_SRC_MAINLIB_MATH_BASISMATRIX_H
