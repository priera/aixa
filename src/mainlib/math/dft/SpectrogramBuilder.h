#ifndef AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMBUILDER_H
#define AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMBUILDER_H

#include "SpectrogramComputer.h"
#include "FourierTransformFactory.h"

namespace aixa::math {
    class SpectrogramBuilder {
    public:
        explicit SpectrogramBuilder(double samplePeriod) :
            samplePeriod(samplePeriod),
            fourierImplementation(FourierTransformFactory::Implementations::FFT),
            transformSize(2048),
            logScale(false) {}

        virtual ~SpectrogramBuilder() noexcept = default;

        SpectrogramBuilder &setFourierTransform(FourierTransformFactory::Implementations impl) {
            this->fourierImplementation = impl;
            return *this;
        }

        SpectrogramBuilder &setTransformSize(unsigned int transformSize) {
            this->transformSize = transformSize;
            return *this;
        }

        SpectrogramBuilder &useLogScale() {
            this->logScale = true;
            return *this;
        }

        SpectrogramComputer *build() const;

    private:
        Scale *buildScale(const FourierTransform &fourierTransform) const;

        double samplePeriod;
        FourierTransformFactory::Implementations fourierImplementation;
        unsigned int transformSize;
        bool logScale;
    };
}


#endif //AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMBUILDER_H
