#ifndef AIXA_SRC_MAINLIB_DSP_VECTOR_H
#define AIXA_SRC_MAINLIB_DSP_VECTOR_H

#include "Matrix.h"

namespace aixa::math {

template <typename T, class ZeroComparer>
class ConstVectorProxy;

template <typename T, class ZeroComparer>
class Vector : public Matrix<T, ZeroComparer> {
public:
    enum class Ordering
    {
        ROW_ORDERED,
        COLUMN_ORDERED
    };

    template <class OtherIt>
    Vector(OtherIt other, std::size_t M, Ordering ordering = Ordering::ROW_ORDERED) :
        Matrix<T, ZeroComparer>(checkColumns(M, ordering), checkRows(M, ordering), std::true_type(), T()),
        ordering(ordering) {
        std::copy(other, other + M, this->content.begin());
    }

    explicit Vector(std::size_t M, Ordering ordering = Ordering::ROW_ORDERED, T def = T()) :
        Matrix<T, ZeroComparer>(checkColumns(M, ordering), checkRows(M, ordering), std::true_type(), def),
        ordering(ordering) {}

    ~Vector() override = default;

    ConstVectorProxy<T, ZeroComparer> slice(std::size_t beginning, std::size_t count) const;
    Vector<T, ZeroComparer> copy(std::size_t beginning, std::size_t count) const;

    virtual const T& operator[](std::size_t n) const { return this->content[n]; }

    T& operator[](std::size_t n) { return this->content[n]; }

    Vector<T, ZeroComparer> operator*(const Vector<T, ZeroComparer>& other) const;

protected:
    Vector(std::size_t M, std::false_type) :
        Matrix<T, ZeroComparer>(checkColumns(M, ordering), checkRows(M, ordering), std::false_type()) {}

private:
    static constexpr std::size_t checkColumns(std::size_t candidateDim, Ordering ordering) {
        return (ordering == Ordering::ROW_ORDERED) ? candidateDim : 1;
    }

    static constexpr std::size_t checkRows(std::size_t candidateDim, Ordering ordering) {
        return (ordering == Ordering::COLUMN_ORDERED) ? candidateDim : 1;
    }

    Ordering ordering;
};
}  // namespace aixa::math

#endif  // AIXA_SRC_MAINLIB_DSP_VECTOR_H
