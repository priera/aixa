#ifndef AIXA_SRC_MAINLIB_DSP_FOURIERTRANSFORM_H
#define AIXA_SRC_MAINLIB_DSP_FOURIERTRANSFORM_H

#include <cmath>

#include "mainlib/math/Matrix.h"
#include "mainlib/math/Vector.h"
#include "mainlib/math/types.h"
#include "FourierTransform.h"

namespace aixa::math {
    class MatrixFourierTransform : public FourierTransform {
    public:
        MatrixFourierTransform(unsigned int N, ComplexBasis basis) :
                FourierTransform(N),
                basis(std::move(basis)) {}

        ~MatrixFourierTransform() override = default;

    protected:
        const ComplexVector &transform(const ComplexVector &complexSignal) override;

    private:
        ComplexBasis basis;

#ifdef QTEST
        friend class MatrixFourierTransform_UTest;
#endif
    };
}

#endif //AIXA_SRC_MAINLIB_DSP_FOURIERTRANSFORM_H
