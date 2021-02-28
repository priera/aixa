#ifndef AIXA_SRC_MAINLIB_DSP_MATRIX_H
#define AIXA_SRC_MAINLIB_DSP_MATRIX_H

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace aixa::math {

template <typename T, class TypeAxioms>
class Matrix {
public:
    // N columns, M rows
    Matrix(size_t N, size_t M, T def = T()) : Matrix(N, M, std::true_type(), def){};

    template <template <typename...> typename SequenceContainer>
    Matrix(size_t N, size_t M, SequenceContainer<T>& content) : columns_(N), rows_(M) {
        this->content.swap(content);
    }

    Matrix(const Matrix<T, TypeAxioms>& other) = default;

    virtual ~Matrix() = default;

    const T& operator()(size_t row, size_t column) const { return content[row * columns_ + column]; }

    T& operator()(size_t row, size_t column) { return content[row * columns_ + column]; }

    Matrix<T, TypeAxioms> operator*(const T t) const {
        Matrix<T, TypeAxioms> ret(*this);

        for (auto& elem : ret.content) elem *= t;

        return std::move(ret);
    }

    Matrix<T, TypeAxioms> x(const Matrix<T, TypeAxioms>& rhs) const {
        Matrix<T, TypeAxioms> ret(rhs.columns(), this->rows());
        multiply(rhs, ret);
        return ret;
    }

    Matrix<T, TypeAxioms> operator+(const Matrix<T, TypeAxioms>& other) const {
        if (this->rows() != other.rows() || this->columns() != other.columns())
            throw std::runtime_error("Invalid matrix operation");

        Matrix<T, TypeAxioms> ret(*this);
        for (size_t i = 0; i < size(); i++) {
            ret.content[i] += other.content[i];
        }

        return std::move(ret);
    }

    Matrix<T, TypeAxioms>& operator+=(const Matrix<T, TypeAxioms>& other) {
        auto sum = *this + other;
        this->content.swap(sum.content);
        return *this;
    }

    Matrix<T, TypeAxioms> operator-(const Matrix<T, TypeAxioms>& other) const {
        if (this->rows() != other.rows() || this->columns() != other.columns())
            throw std::runtime_error("Invalid matrix operation");

        TypeAxioms axioms;
        auto diff = (*this) + (other * axioms.inverter());

        return std::move(diff);
    }

    bool operator==(const Matrix<T, TypeAxioms>& other) const {
        if (this->rows() != other.rows() || this->columns() != other.columns())
            throw std::runtime_error("Invalid matrix operation");

        bool eq = true;
        for (size_t i = 0; i < size(); i++) {
            if (this->content[i] != other.content[i]) {
                eq = false;
                break;
            }
        }

        return eq;
    }

    bool operator!=(const Matrix<T, TypeAxioms>& other) const { return !(*this == other); }

    size_t columns() const { return columns_; }

    size_t rows() const { return rows_; }

    std::vector<T> vector(size_t row) const {
        auto beginOffset = row * columns();
        auto end = (row + 1) * columns();
        return std::vector<T>(content.begin() + beginOffset, content.begin() + end);
    }

    const std::vector<T>& constContent() const { return content; }

    size_t size() const { return rows_ * columns_; }

    void print() const;

protected:
    Matrix(size_t N, size_t M, std::true_type allocate, T def);
    Matrix(size_t N, size_t M, std::false_type resize);

    void multiply(const Matrix<T, TypeAxioms>& other, Matrix<T, TypeAxioms>& result) const;

    size_t columns_;
    size_t rows_;

    std::vector<T> content;
};

// template <typename T, class TypeAxioms>
// Matrix<T, TypeAxioms> operator*(const Matrix<T, TypeAxioms>& rhs) const {
//    Matrix<T, TypeAxioms> ret(rhs.columns(), this->rows());
//    multiply(rhs, ret);
//    return ret;
//}

}  // namespace aixa::math

#endif  // AIXA_SRC_MAINLIB_DSP_MATRIX_H
