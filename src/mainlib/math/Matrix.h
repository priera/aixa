#ifndef AIXA_SRC_MAINLIB_DSP_MATRIX_H
#define AIXA_SRC_MAINLIB_DSP_MATRIX_H


#include <cstddef>
#include <vector>
#include <stdexcept>

namespace aixa::math {

    template<typename T>
    class Matrix {
    public:

        //N columns, M rows
        Matrix(size_t N, size_t M, T def = T());

        Matrix(Matrix &other) = default;
        Matrix(Matrix &&other) noexcept = default;

        virtual ~Matrix() = default;

        const T &operator()(size_t row, size_t column) const {
            return content[row * columns_ + column];
        }

        T &operator()(size_t row, size_t column) {
            return content[row * columns_ + column];
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

        void multiply(const Matrix<T> &other, Matrix<T> &result);

        void print() const;

    protected:
        size_t columns_;
        size_t rows_;

        std::vector<T> content;
    };
}


#endif //AIXA_SRC_MAINLIB_DSP_MATRIX_H
