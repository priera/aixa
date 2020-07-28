#ifndef AIXA_SRC_MAINLIB_MATH_COOLEYTUKEYFFT_H
#define AIXA_SRC_MAINLIB_MATH_COOLEYTUKEYFFT_H

#include <utility>

#include "FourierTransform.h"

namespace aixa::math {

    class CooleyTukeyFFT : public FourierTransform {
    public:
        explicit CooleyTukeyFFT(unsigned int N,
                                unsigned int baseWeightPower,
                                std::vector<size_t> indirections,
                                std::vector<Complex> W) :
                FourierTransform(N),
                baseWeightPower(baseWeightPower),
                indirections(std::move(indirections)),
                W(std::move(W)),
                result(N),
                temp(N / 2){};

        ~CooleyTukeyFFT() override = default;

    protected:
        const ComplexVector &transform(const ComplexVector &complexSignal) override;

    private:
        unsigned int baseWeightPower;
        std::vector<size_t> indirections;
        std::vector<Complex> W;

        ComplexVector result;
        std::vector<Complex> temp;
    };
}


#endif //AIXA_SRC_MAINLIB_MATH_COOLEYTUKEYFFT_H
