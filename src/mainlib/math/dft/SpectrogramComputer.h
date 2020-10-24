#ifndef AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H
#define AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H

#include <memory>

#include "FourierTransform.h"

namespace aixa::math {
    class SpectrogramComputer {
    public:
        struct SpectralSlices {
            std::vector<ComplexVector> slices;
        };

        explicit SpectrogramComputer(std::unique_ptr<FourierTransform> fourierTransform) :
                fourierTransform(std::move(fourierTransform)),
                windowSize(this->fourierTransform->dimensionality()),
                overlapping(windowSize / 2),
                slicesCount(0),
                remainder(nullptr)
            {}

        virtual ~SpectrogramComputer() noexcept = default;

        void computeOn(DoubleVector &samples);

    private:
        bool initialized() const {
            return static_cast<bool>(remainder);
        }

        void init(size_t samplesSize);

        std::unique_ptr<FourierTransform> fourierTransform;
        unsigned int windowSize;
        unsigned int overlapping;

        unsigned int slicesCount;
        std::unique_ptr<DoubleVector> remainder;
    };
}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H

