#ifndef AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H
#define AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H

#include <memory>

#include "FourierTransform.h"
#include "SpectrogramFragment.h"

namespace aixa::math {
    class SpectrogramComputer : public SpectrogramGenerator {
    public:
        explicit SpectrogramComputer(std::unique_ptr<FourierTransform> fourierTransform) :
                SpectrogramGenerator(),
                fourierTransform(std::move(fourierTransform)),
                windowSize(this->fourierTransform->dimensionality()),
                overlapping(windowSize / 2),
                slicesCount(0),
                remainder(nullptr) {}

        ~SpectrogramComputer() noexcept override = default;

        void computeOn(const DoubleVector &samples);

    private:
        bool initialized() const {
            return static_cast<bool>(remainder);
        }

        void init(size_t samplesSize);

        std::vector<double> extractMagnitude(const ComplexVector &transformResult) const;

        std::unique_ptr<FourierTransform> fourierTransform;
        unsigned int windowSize;
        unsigned int overlapping;

        unsigned int slicesCount;
        std::unique_ptr<DoubleVector> remainder;
    };
}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H

