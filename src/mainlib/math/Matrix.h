#ifndef AIXA_SRC_MAINLIB_DSP_MATRIX_H
#define AIXA_SRC_MAINLIB_DSP_MATRIX_H

#include <array>
#include <cstddef>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include <platform/aixa_export.h>

namespace aixa::math {

template <typename T, class TypeAxioms>
class LIB_EXPORT Matrix {
public:
    // N columns, M rows
    Matrix(std::size_t N, std::size_t M, T def = T()) : Matrix(N, M, std::true_type(), def){};

    template <std::size_t ArrayDim>
    Matrix(std::size_t N, std::size_t M, const std::array<T, ArrayDim>& content) :
        Matrix(N, M, std::false_type()) {
        std::ranges::copy(content, std::back_inserter(this->content_));
    }

    Matrix(const Matrix<T, TypeAxioms>& other) = default;
    Matrix(Matrix<T, TypeAxioms>&& other) noexcept = default;

    virtual ~Matrix() = default;

    Matrix<T, TypeAxioms>& operator=(Matrix<T, TypeAxioms>&& other) noexcept = default;

    const T& operator()(std::size_t row, std::size_t column) const {
        return content_[row * columns_ + column];
    }

    T& operator()(std::size_t row, std::size_t column) { return content_[row * columns_ + column]; }

    Matrix<T, TypeAxioms> operator*(T t) const {
        Matrix<T, TypeAxioms> ret(*this);

        for (auto& elem : ret.content_) elem *= t;

        return std::move(ret);
    }

    Matrix<T, TypeAxioms> operator*(const Matrix<T, TypeAxioms>& rhs) const {
        Matrix<T, TypeAxioms> ret(rhs.columns(), this->rows());
        multiply(rhs, ret);
        return ret;
    }

    Matrix<T, TypeAxioms>& operator*=(const Matrix<T, TypeAxioms>& rhs) {
        Matrix<T, TypeAxioms> ret(rhs.columns(), this->rows());
        multiply(rhs, ret);
        *this = std::move(ret);
        return *this;
    }

    Matrix<T, TypeAxioms> operator+(const Matrix<T, TypeAxioms>& other) const {
        if (this->rows() != other.rows() || this->columns() != other.columns())
            throw std::runtime_error("Invalid matrix operation");

        Matrix<T, TypeAxioms> ret(*this);
        for (std::size_t i = 0; i < size(); i++) {
            ret.content_[i] += other.content_[i];
        }

        return std::move(ret);
    }

    void elemWiseProduct(const Matrix<T, TypeAxioms>& other) {
        if (!(this->rows() == other.rows() && this->columns() == other.columns()))
            throw std::runtime_error("Invalid matrix operation");

        const auto size = this->size();
        for (std::size_t i = 0; i < size; i++) {
            this->content_[i] *= other.content_[i];
        }
    }

    Matrix<T, TypeAxioms> elemWiseProduct(const Matrix<T, TypeAxioms>& other) const {
        auto ret = *this;
        ret.elemWiseProduct(other);
        return ret;
    }

    Matrix<T, TypeAxioms>& operator+=(const Matrix<T, TypeAxioms>& other) {
        auto sum = *this + other;
        this->content_.swap(sum.content_);
        return *this;
    }

    Matrix<T, TypeAxioms> operator-(const Matrix<T, TypeAxioms>& other) const {
        if (this->rows() != other.rows() || this->columns() != other.columns())
            throw std::runtime_error("Invalid matrix operation");

        TypeAxioms axioms;
        return (*this) + (other * axioms.inverter());
    }

    bool operator==(const Matrix<T, TypeAxioms>& other) const {
        if (this->rows() != other.rows() || this->columns() != other.columns())
            throw std::runtime_error("Invalid matrix operation");

        bool eq = true;
        for (std::size_t i = 0; i < size(); i++) {
            if (this->content_[i] != other.content_[i]) {
                eq = false;
                break;
            }
        }

        return eq;
    }

    bool operator!=(const Matrix<T, TypeAxioms>& other) const { return !(*this == other); }

    Matrix<T, TypeAxioms> transpose() const {
        auto ret = Matrix<T, TypeAxioms>(rows(), columns());
        for (std::size_t row = 0; row < rows(); row++) {
            for (std::size_t col = 0; col < columns(); col++) {
                ret(col, row) = (*this)(row, col);
            }
        }

        return ret;
    }

    std::size_t columns() const { return columns_; }

    std::size_t rows() const { return rows_; }

    std::vector<T> row(std::size_t rowInd) const {
        auto beginOffset = rowInd * columns();
        auto end = (rowInd + 1) * columns();
        return std::vector<T>(content_.begin() + beginOffset, content_.begin() + end);
    }

    const std::vector<T>& content() const { return content_; }
    std::vector<T>& content() { return content_; }

    std::size_t size() const { return rows_ * columns_; }

    void print() const;

    Matrix<T, TypeAxioms> collapseRows() const {
        auto ret = Matrix<T, TypeAxioms>(1, rows());
        for (std::size_t row = 0; row < rows(); row++) {
            T sum = T();
            for (std::size_t col = 0; col < columns(); col++) {
                sum += (*this)(row, col);
            }
            ret(row, 0) = sum;
        }

        return ret;
    }

protected:
    Matrix(std::size_t N, std::size_t M, std::true_type allocate, T def);
    Matrix(std::size_t N, std::size_t M, std::false_type resize);

    void multiply(const Matrix<T, TypeAxioms>& other, Matrix<T, TypeAxioms>& result) const;

    std::size_t columns_;
    std::size_t rows_;

    std::vector<T> content_;
};

template <typename T, typename TypeAxioms>
Matrix<T, TypeAxioms> operator*(T t, const Matrix<T, TypeAxioms>& rhs) {
    return rhs * t;
}

}  // namespace aixa::math

#endif  // AIXA_SRC_MAINLIB_DSP_MATRIX_H
