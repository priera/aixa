#include "Vector.h"

#include "ConstVectorProxy.h"
#include "typeAxioms.h"

namespace aixa::math {

    template <typename T, class TypeAxioms>
    ConstVectorProxy<T, TypeAxioms> Vector<T, TypeAxioms>::slice(std::size_t beginning, std::size_t count) const {
        assert(beginning + count < this->content.size());

        return ConstVectorProxy<T, TypeAxioms>(this->content, beginning, count);
    }

    template<typename T, class ZeroComparer>
    Vector<T, ZeroComparer> Vector<T, ZeroComparer>::copy(std::size_t beginning, std::size_t count) const {
        return Vector<T, ZeroComparer>(this->content.begin() + beginning, count);
    }

    template<typename T, class ZeroComparer>
    Vector<T, ZeroComparer> Vector<T, ZeroComparer>::operator*(const Vector<T, ZeroComparer> &other) const {
        //This should be implemented at matrix level. Consider using CRTP
        Vector<T, ZeroComparer> result(this->rows());
        for (std::size_t i = 0; i < this->content.size(); i++) {
            result[i] = this->content[i] * other.content[i];
        }

        return result;
    }

    template class Vector<float, DoubleTypeAxioms>;
    template class Vector<double, DoubleTypeAxioms>;
    template class Vector<Complex, ComplexTypeAxioms>;
}