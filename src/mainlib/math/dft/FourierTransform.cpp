#include "FourierTransform.h"

namespace aixa::math {

    const ComplexVector &FourierTransform::applyTo(const DoubleVector &signal) {
        auto complexSignal = fromRealVector(signal);
        return transform(complexSignal);
    }
}