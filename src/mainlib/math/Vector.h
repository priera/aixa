#ifndef AIXA_SRC_MAINLIB_DSP_VECTOR_H
#define AIXA_SRC_MAINLIB_DSP_VECTOR_H

#include "Matrix.h"

namespace aixa::math {

template <typename T, class ZeroComparer>
class ConstVectorProxy;

template <typename T, class ZeroComparer>
class Vector : public Matrix<T, ZeroComparer> {
public:
    template <class OtherIt>
    Vector(OtherIt other, size_t M, T def = T()) : Matrix<T, ZeroComparer>(1, M, std::true_type(), def) {
        std::copy(other, other + M, this->content.begin());
    }

    explicit Vector(size_t M, T def = T()) : Matrix<T, ZeroComparer>(1, M, std::true_type(), def) {}

    template <template <typename...> typename SequenceContainer>
    explicit Vector(SequenceContainer<T>& content) : Matrix<T, ZeroComparer>(1, content.size(), content) {}

    ~Vector() override = default;

    ConstVectorProxy<T, ZeroComparer> slice(std::size_t beginning, std::size_t count) const;
    Vector<T, ZeroComparer> copy(std::size_t beginning, std::size_t count) const;

    virtual const T& operator[](std::size_t n) const { return this->content[n]; }

    T& operator[](std::size_t n) { return this->content[n]; }

    Vector<T, ZeroComparer> operator*(const Vector<T, ZeroComparer>& other) const;

protected:
    Vector(size_t M, std::false_type) : Matrix<T, ZeroComparer>(1, M, std::false_type()) {}
};
}  // namespace aixa::math

#endif  // AIXA_SRC_MAINLIB_DSP_VECTOR_H
