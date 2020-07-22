#include "types.h"

namespace aixa::math {
    template class Vector<double>;
    template class Vector<float>;
    template class Vector<Complex>;

    ComplexVector fromRealVector(const DoubleVector &realVector) {
        ComplexVector ret(realVector.size());
        for (size_t i = 0; i < realVector.size(); i++) {
            ret(i) = realVector(i);
        }

        return std::move(ret);
    }
}