#ifndef AIXA_AIXA_SRC_MAINLIB_MATH_MATRIX2_H
#define AIXA_AIXA_SRC_MAINLIB_MATH_MATRIX2_H

#include "InvertibleNumber.h"

#include <vector>
#include <iostream>

template<InvertibleNumber Invertible>
class Matrix2 {
public:
    using T = typename Invertible::type;

    // N columns, M rows
    Matrix2(std::size_t N, std::size_t M, T def = T()) :
        Matrix2(N, M, std::true_type(), def){};

    const T& operator()(std::size_t row, std::size_t column) const {
        return content_[row * columns_ + column];
    }

    T& operator()(std::size_t row, std::size_t column) { return content_[row * columns_ + column]; }

    Matrix2<Invertible> operator*(T t) const {
        Matrix2<Invertible> ret(*this);

        for (auto& elem : ret.content_) elem *= t;

        return std::move(ret);
    }

    Matrix2<Invertible> operator*(const Matrix2<Invertible>& rhs) const {
        Matrix2<Invertible> ret(rhs.columns(), this->rows());
        multiply(*this, rhs, ret);
        return ret;
    }

    Matrix2<Invertible>& operator*=(const Matrix2<Invertible>& rhs) {
        Matrix2<Invertible> ret(rhs.columns(), this->rows());
        multiply(*this, rhs, ret);
        *this = std::move(ret);
        return *this;
    }

    Matrix2<Invertible> operator+(const Matrix2<Invertible>& other) const {
        if (this->rows() != other.rows() || this->columns() != other.columns())
            throw std::runtime_error("Invalid matrix operation");

        Matrix2<Invertible> ret(*this);
        for (std::size_t i = 0; i < size(); i++) {
            ret.content_[i] += other.content_[i];
        }

        return std::move(ret);
    }

    void elemWiseProduct(const Matrix2<Invertible>& other) {
        if (!(this->rows() == other.rows() && this->columns() == other.columns()))
            throw std::runtime_error("Invalid matrix operation");

        const auto size = this->size();
        for (std::size_t i = 0; i < size; i++) {
            this->content_[i] *= other.content_[i];
        }
    }

    Matrix2<Invertible> elemWiseProduct(const Matrix2<Invertible>& other) const {
        auto ret = *this;
        ret.elemWiseProduct(other);
        return ret;
    }

    Matrix2<Invertible>& operator+=(const Matrix2<Invertible>& other) {
        auto sum = *this + other;
        this->content_.swap(sum.content_);
        return *this;
    }

    Matrix2<Invertible> operator-(const Matrix2<Invertible>& other) const {
        if (this->rows() != other.rows() || this->columns() != other.columns())
            throw std::runtime_error("Invalid matrix operation");

        InvertibleNumber auto inverter = Invertible();
        return (*this) + (other * inverter());
    }

    bool operator==(const Matrix2<Invertible>& other) const {
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

    bool operator!=(const Matrix2<Invertible>& other) const { return !(*this == other); }

    Matrix2<Invertible> transpose() const {
        auto ret = Matrix2<Invertible>(rows(), columns());
        for (std::size_t row = 0; row < rows(); row++) {
            for (std::size_t col = 0; col < columns(); col++) {
                ret(col, row) = (*this)(row, col);
            }
        }

        return ret;
    }

    Matrix2<Invertible> collapseRows() const {
        auto ret = Matrix2<Invertible>(1, rows());
        for (std::size_t row = 0; row < rows(); row++) {
            T sum = T();
            for (std::size_t col = 0; col < columns(); col++) {
                sum += (*this)(row, col);
            }
            ret(row, 0) = sum;
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

protected:
    Matrix2(std::size_t N, std::size_t M, std::true_type allocate, T def) :
            columns_(N), rows_(M) {
        content_.resize(columns_ * rows_, def);
    }

    static void multiply(const Matrix2<Invertible>& lhs, const Matrix2<Invertible>& rhs, Matrix2<Invertible>& result) {
        if (lhs.columns() != rhs.rows() || lhs.rows() != result.rows() ||
            rhs.columns() != result.columns()) {
            throw std::runtime_error("Invalid matrix multiplication");
        }

        // The most naive implementation: O(n³) complexity and awful cache performance
        for (std::size_t r = 0; r < lhs.rows(); r++) {
            for (std::size_t c_r = 0; c_r < rhs.columns(); c_r++) {
                T acc = T();
                for (std::size_t c = 0; c < lhs.columns(); c++) {
                    acc += lhs(r, c) * rhs(c, c_r);
                }

                result(r, c_r) = acc;
            }
        }
    }

    std::size_t columns_;
    std::size_t rows_;

    std::vector<T> content_;
};

template<InvertibleNumber Invertible>
std::ostream& operator<<(std::ostream& ostream, const Matrix2<Invertible>& m) {
    const auto& content = m.content();
    for (std::size_t row = 0; row < m.rows(); row++) {
        for (std::size_t col = 0; col < m.columns(); col++) {
            ostream << content[row * m.columns() + col] << " ";
        }
        ostream << "\n";
    }
    ostream << "\n";

    return ostream;
}

#endif //AIXA_AIXA_SRC_MAINLIB_MATH_MATRIX2_H
