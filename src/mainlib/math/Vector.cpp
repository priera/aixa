#include "Vector.h"

#include "ConstVectorProxy.h"
#include "typeAxioms.h"

namespace aixa::math {

    template <typename T, class TypeAxioms>
    ConstVectorProxy<T, TypeAxioms> Vector<T, TypeAxioms>::slice(std::size_t beginning, std::size_t count) const {
        assert(beginning + count < this->content.size());

        return ConstVectorProxy<T, TypeAxioms>(this->content, beginning, count);
    }

    template class Vector<float, DoubleTypeAxioms>;
    template class Vector<double, DoubleTypeAxioms>;
    template class Vector<Complex, ComplexTypeAxioms>;
}