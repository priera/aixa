#ifndef AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H
#define AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H

#include <memory>

#include <mainlib/math/scale/Scale.h>

#include "FourierTransform.h"
#include "SpectrogramFragment.h"

namespace aixa::math {
    class SpectrogramComputer : public SpectrogramGenerator {
    public:
        explicit SpectrogramComputer(std::unique_ptr<FourierTransform> fourierTransform,
                                     std::unique_ptr<Scale> scale) :
                SpectrogramGenerator(),
                fourierTransform(std::move(fourierTransform)),
                scale(std::move(scale)),
                windowSize(this->fourierTransform->dimensionality()),
                sliceSize(this->fourierTransform->relevantSize()),
                overlapping(windowSize / 2),
                mask(windowSize),
                slicesCount(0),
                remainder(nullptr) {
            buildMask();
        }

        ~SpectrogramComputer() noexcept override = default;

        void computeOn(const DoubleVector &samples);

    private:
        bool initialized() const {
            return static_cast<bool>(remainder);
        }

        void init(size_t samplesSize);

        void buildMask();

        void storeMagnitude(const ComplexVector &result, std::vector<double>& storeAt) const;

        std::unique_ptr<FourierTransform> fourierTransform;
        std::unique_ptr<Scale> scale;

        unsigned int windowSize;
        unsigned int overlapping;
        DoubleVector mask;
        std::size_t sliceSize;

        unsigned int slicesCount;
        std::unique_ptr<DoubleVector> remainder;
    };
}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMCOMPUTER_H

