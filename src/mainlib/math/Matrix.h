#ifndef AIXA_SRC_MAINLIB_DSP_MATRIX_H
#define AIXA_SRC_MAINLIB_DSP_MATRIX_H


#include <cstddef>
#include <vector>
#include <stdexcept>

namespace aixa::math {

    template<typename T,
            class TypeAxioms>
    class Matrix {
    public:

        //N columns, M rows
        Matrix(size_t N, size_t M, T def = T());
        Matrix(const Matrix<T, TypeAxioms> &other) = default;

        virtual ~Matrix() = default;

        const T &operator()(size_t row, size_t column) const {
            return content[row * columns_ + column];
        }

        T &operator()(size_t row, size_t column) {
            return content[row * columns_ + column];
        }

        Matrix<T, TypeAxioms> operator*(const T t) const {
            Matrix<T, TypeAxioms> ret(*this);

            for (auto& elem: ret.content)
                elem *= t;

            return std::move(ret);
        }

        Matrix<T, TypeAxioms> operator+(const Matrix<T, TypeAxioms>& other) const {
            if (this->rows() != other.rows() ||
                this->columns() != other.columns())
                throw std::runtime_error("Invalid matrix operation");

            Matrix<T, TypeAxioms> ret(*this);
            for (size_t i = 0; i < size(); i++) {
                ret.content[i] += other.content[i];
            }

            return std::move(ret);
        }

        Matrix<T, TypeAxioms> &operator+=(const Matrix<T, TypeAxioms>& other) {
            auto sum = *this + other;
            this->content.swap(sum.content);
            return *this;
        }

        Matrix<T, TypeAxioms> operator-(const Matrix<T, TypeAxioms>& other) const {
            if (this->rows() != other.rows() ||
                this->columns() != other.columns())
                throw std::runtime_error("Invalid matrix operation");

            TypeAxioms axioms;
            auto diff = (*this) + (other * axioms.inverter());

            return std::move(diff);
        }

        bool operator==(const Matrix<T, TypeAxioms>& other) const {
            if (this->rows() != other.rows() ||
                this->columns() != other.columns())
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

        bool operator!=(const Matrix<T, TypeAxioms>& other) const {
            return !(*this == other);
        }

        size_t columns() const {
            return columns_;
        }

        size_t rows() const {
            return rows_;
        }

        //Would be nice to use custom Vector type...
        std::vector<T> vector(size_t m) const {
            std::vector<T> ret(columns_);
            for (size_t col = 0; col < columns_; col++) {
                ret[col] = content[m * columns_ + col];
            }

            return ret;
        }

        size_t size() const {
            return rows_ * columns_;
        }

        void multiply(const Matrix<T, TypeAxioms> &other,
                      Matrix<T, TypeAxioms> &result);

        void print() const;

    protected:
        size_t columns_;
        size_t rows_;

        std::vector<T> content;
    };
}


#endif //AIXA_SRC_MAINLIB_DSP_MATRIX_H
