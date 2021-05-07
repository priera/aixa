#ifndef AIXA_SRC_MAINLIB_MATH_DFT_FFTFACTORY_H
#define AIXA_SRC_MAINLIB_MATH_DFT_FFTFACTORY_H

#include <platform/aixa_export.h>

#include "FourierTransformFactory.h"

namespace aixa::math {
    class LIB_EXPORT FFTFactory : public FourierTransformFactory {
    public:
        FourierTransform *build(unsigned int N) override;

    private:
        std::vector<size_t> splitIndices(size_t N);
        void splitIndicesRec(size_t length,
                             std::vector<size_t>::iterator begin,
                             std::vector<size_t>::iterator end);

        std::vector<Complex> computeWeights(size_t N);

#ifdef QTEST
        friend class FFTFactory_UTest;
#endif
    };

}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_FFTFACTORY_H
