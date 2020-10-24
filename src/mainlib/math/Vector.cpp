#include "Vector.h"

#include "VectorProxy.h"
#include "typeAxioms.h"

namespace aixa::math {

    template <typename T, class TypeAxioms>
    VectorProxy<T, TypeAxioms> Vector<T, TypeAxioms>::slice(std::size_t beginning, std::size_t count) {
        assert(beginning + count < this->content.size());

        return VectorProxy<T, TypeAxioms>(*this, this->content, beginning, count);
    }

    template class Vector<float, DoubleTypeAxioms>;
    template class Vector<double, DoubleTypeAxioms>;
    template class Vector<Complex, ComplexTypeAxioms>;
}