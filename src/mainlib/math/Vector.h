#ifndef AIXA_SRC_MAINLIB_DSP_VECTOR_H
#define AIXA_SRC_MAINLIB_DSP_VECTOR_H

#include "Matrix.h"

namespace aixa::math {
    template<typename T, class ZeroComparer>
    class Vector : public Matrix<T, ZeroComparer> {
    public:
        explicit Vector(size_t M, T def = T()) : Matrix<T, ZeroComparer>(1, M, def) {
        }

        const T &operator[](size_t n) const {
            return this->content[n];
        }

        T &operator[](size_t n) {
            return this->content[n];
        }
    };
}

#endif //AIXA_SRC_MAINLIB_DSP_VECTOR_H
