#include "SpectrogramComputer.h"

#include <mainlib/math/ConstVectorProxy.h>

namespace aixa::math {

    void SpectrogramComputer::computeOn(const DoubleVector &samples) {
        if (!initialized()) {
            init(samples.size());
        } // else: adjust indexes and offsets

        SpectralSlices spectre;

        unsigned int startOffset = 0;
        for (unsigned int i = 0; i < slicesCount; i++) {
            const auto slice = samples.slice(startOffset, windowSize);
            const auto& result = fourierTransform->applyTo(slice);
            spectre.slices.emplace_back(result.slice(0, windowSize / 2).detach());

            startOffset += overlapping;
        }

    }

    void SpectrogramComputer::init(size_t samplesSize) {
        slicesCount = ((samplesSize - windowSize) / overlapping) + 1;
        auto remainderSamples = (samplesSize - windowSize) % overlapping;
        remainder = std::make_unique<DoubleVector>(remainderSamples);
    }

}

