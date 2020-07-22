#include "Matrix.h"
#include "Complex.h"

#include <iostream>

namespace aixa::math {
    template<typename T>
    Matrix<T>::Matrix(size_t N, size_t M, T def) :
            columns_(N), rows_(M) {
        content.resize(columns_ * rows_, def);
    }

    template<typename T>
    void Matrix<T>::multiply(const Matrix<T> &other, Matrix<T> &result) {
        if (this->columns() != other.rows() ||
            this->rows() != result.rows() ||
            other.columns() != result.columns()) {
            throw std::runtime_error("Invalid matrix multiplication");
        }

        //The most naive implementation: O(n³) complexity and awful cache performance
        for (size_t r = 0; r < rows(); r++) {
            for (size_t c_r = 0; c_r < other.columns(); c_r++) {
                T acc = T();
                for (size_t c = 0; c < columns(); c++) {
                    acc += (*this)(r, c) * other(c, c_r);
                }

                result(r, c_r) = acc;
            }
        }
    }

    template<typename T>
    void Matrix<T>::print() const {
        for (size_t r = 0; r < rows(); r++) {
            for (size_t c = 0; c < columns(); c++) {
                std::cout << (*this)(r, c) << " ";
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }

    template class Matrix<float>;
    template class Matrix<double>;
    template class Matrix<Complex>;
}
