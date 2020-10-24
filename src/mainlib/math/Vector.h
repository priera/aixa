#ifndef AIXA_SRC_MAINLIB_DSP_VECTOR_H
#define AIXA_SRC_MAINLIB_DSP_VECTOR_H

#include "Matrix.h"

namespace aixa::math {

    template<typename T, class ZeroComparer>
    class VectorProxy;

    template<typename T, class ZeroComparer>
    class Vector : public Matrix<T, ZeroComparer> {
    public:
        template<class Other>
        Vector(Other *other, size_t M, T def = T()) :
            Matrix<T, ZeroComparer>(1, M, std::true_type(), def) {
            std::copy(other, other + M, this->content.begin());
        }

        explicit Vector(size_t M, T def = T()) :
            Matrix<T, ZeroComparer>(1, M, std::true_type(), def) { }

        ~Vector() override = default;

        VectorProxy<T, ZeroComparer> slice(std::size_t beginning, std::size_t count);

        virtual const T &operator[](std::size_t n) const {
            return this->content[n];
        }

        virtual T &operator[](std::size_t n) {
            return this->content[n];
        }

    protected:
        Vector(size_t M, std::false_type) :
                Matrix<T, ZeroComparer>(1, M, std::false_type()) {
        }
    };
}

#endif //AIXA_SRC_MAINLIB_DSP_VECTOR_H
