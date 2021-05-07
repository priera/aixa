#include "Matrix.h"

#include <iostream>

#include "Complex.h"
#include "typeAxioms.h"

namespace aixa::math {
template <typename T, class TypeAxioms>
Matrix<T, TypeAxioms>::Matrix(std::size_t N, std::size_t M, std::true_type allocate, T def) :
    columns_(N), rows_(M) {
    content_.resize(columns_ * rows_, def);
}

template <typename T, class TypeAxioms>
Matrix<T, TypeAxioms>::Matrix(std::size_t N, std::size_t M, std::false_type allocate) :
    columns_(N), rows_(M) {}

template <typename T, class TypeAxioms>
void Matrix<T, TypeAxioms>::multiply(const Matrix<T, TypeAxioms> &other,
                                     Matrix<T, TypeAxioms> &result) const {
    if (this->columns() != other.rows() || this->rows() != result.rows() ||
        other.columns() != result.columns()) {
        throw std::runtime_error("Invalid matrix multiplication");
    }

    // The most naive implementation: O(n³) complexity and awful cache performance
    for (std::size_t r = 0; r < rows(); r++) {
        for (std::size_t c_r = 0; c_r < other.columns(); c_r++) {
            T acc = T();
            for (std::size_t c = 0; c < columns(); c++) {
                acc += (*this)(r, c) * other(c, c_r);
            }

            result(r, c_r) = acc;
        }
    }
}

template <typename T, class TypeAxioms>
void Matrix<T, TypeAxioms>::print() const {
    for (std::size_t r = 0; r < rows(); r++) {
        for (std::size_t c = 0; c < columns(); c++) {
            std::cout << (*this)(r, c) << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

template class Matrix<float, DoubleTypeAxioms>;
template class Matrix<double, DoubleTypeAxioms>;
template class Matrix<Complex, ComplexTypeAxioms>;
}  // namespace aixa::math
