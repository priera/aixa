#include "types.h"

namespace aixa::math {
    template class Vector<double, DoubleTypeAxioms>;
    template class Vector<float, DoubleTypeAxioms>;
    template class Vector<Complex, ComplexTypeAxioms>;

    ComplexVector fromRealVector(const DoubleVector &realVector) {
        ComplexVector ret(realVector.size());
        for (size_t i = 0; i < realVector.size(); i++) {
            ret[i] = realVector[i];
        }

        return ret;
    }
}