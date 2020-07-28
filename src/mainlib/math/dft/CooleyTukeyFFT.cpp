#include "CooleyTukeyFFT.h"

#include <cmath>

namespace aixa::math {

    /** Sequential implementation of the Cooley-Tukey Fast Fourier Transform
     * The reasons for using a sequential version instead of a recursive one are:
     * - avoid the performance penalty of recursive function calls
     * - let the compiler to perform optimizations on the loops
     * Also, this implementation tries to minimize both the amount of memory required for storing temporary results
     * and the number of scaling factors Wn computed
     *  */
    const ComplexVector &CooleyTukeyFFT::transform(const ComplexVector &complexSignal) {
        for (size_t i = 0; i < N; i++) {
            result[i] = complexSignal[indirections[i]];
        }

        const Complex invert = -1;

        unsigned int butterflies = N >> 1u;
        unsigned int butterflySize = 2;
        unsigned int halfLength = 1;
        unsigned int power = baseWeightPower;

        while (butterflies > 0) {
            for (unsigned int fly = 0; fly < butterflies; fly++) {
                auto currButterfly = fly * butterflySize;
                auto currOddStart = currButterfly + halfLength;

                for (unsigned int even = 0; even < halfLength; even++) {
                    temp[even] = result[currButterfly + even];
                }

                for (unsigned int k = 0; k < halfLength; k++) {
                    result[currOddStart + k] *= W[k * power];
                }

                for (unsigned int even = 0; even < halfLength; even++) {
                    result[currButterfly + even] += result[currOddStart + even];
                }

                for (unsigned int odd = 0; odd < halfLength; odd++) {
                    auto inverted = (invert * result[currOddStart + odd]);
                    result[currOddStart + odd] = temp[odd] + inverted;
                }
            }

            butterflies >>= 1u;
            butterflySize <<= 1u;
            halfLength <<= 1u;
            power >>= 1u;
        }

        return result;
    }
}

