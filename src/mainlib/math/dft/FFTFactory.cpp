#include "FFTFactory.h"
#include "CooleyTukeyFFT.h"

#include <cmath>
#include <mainlib/math/ComplexExponential.h>

namespace aixa::math {
    FourierTransform* FFTFactory::build(unsigned int N) {
        auto log = std::log2(static_cast<double>(N));

        double power;
        auto fractional = std::abs(std::modf(log, &power));
        if (fractional > 1e-3) {
            throw std::invalid_argument("Parameter to Cooley-Tukey must be a power of 2");
        }

        auto basePower = static_cast<unsigned int>(std::pow(2, power - 1));
        auto indices = splitIndices(N);
        auto weights = computeWeights(N);

        return new CooleyTukeyFFT(N, basePower, indices, weights);
    }

    std::vector<size_t> FFTFactory::splitIndices(size_t N) {
        auto ret = std::vector<size_t>(N);
        for (size_t i = 0; i < N; i++) {
            ret[i] = i;
        }

        splitIndicesRec(N, ret.begin(), ret.end());
        return ret;
    }

    void FFTFactory::splitIndicesRec(size_t length,
                                         std::vector<size_t>::iterator begin,
                                         std::vector<size_t>::iterator end) {
        if (length > 2) {
            auto temp = std::vector<size_t>(begin, end);
            auto halfLength = length / 2;

            for (size_t i = 0; i < halfLength; i++) {
                begin[i] = begin[i * 2];
            }
            for (size_t i = 0; i < halfLength; i++) {
                begin[halfLength + i] = temp[(i * 2) + 1];
            }

            splitIndicesRec(halfLength, begin, begin + halfLength);
            splitIndicesRec(halfLength, begin + halfLength, end);
        }
    }

    std::vector<Complex> FFTFactory::computeWeights(size_t N) {
        auto count = N / 2;
        auto ret = std::vector<Complex>(count);

        double fundamentalFrequency = (2 * M_PI) / N;

        for (size_t k = 0; k < count; k++) {
            auto ex = ~(ComplexExponential(fundamentalFrequency * k));
            ret[k] = Complex(ex.re(), ex.im());
        }

        return ret;
    }
}
