#include "Vector.h"

#include "ConstVectorProxy.h"
#include "typeAxioms.h"

namespace aixa::math {

template <typename T, class TypeAxioms>
ConstVectorProxy<T, TypeAxioms> Vector<T, TypeAxioms>::slice(std::size_t beginning, std::size_t count) const {
    assert(beginning + count < this->content_.size());

    return ConstVectorProxy<T, TypeAxioms>(this->content_, beginning, count);
}

template <typename T, class ZeroComparer>
Vector<T, ZeroComparer> Vector<T, ZeroComparer>::copy(std::size_t beginning, std::size_t count) const {
    return Vector<T, ZeroComparer>(this->content_.begin() + beginning, count);
}

template class Vector<float, DoubleTypeAxioms>;
template class Vector<double, DoubleTypeAxioms>;
template class Vector<Complex, ComplexTypeAxioms>;
}  // namespace aixa::math