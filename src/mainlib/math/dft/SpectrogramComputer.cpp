#include "SpectrogramComputer.h"

#include <mainlib/math/ConstVectorProxy.h>

namespace aixa::math {

    void SpectrogramComputer::computeOn(const DoubleVector &samples) {
        if (!initialized()) {
            init(samples.size());
        } // else: adjust indexes and offsets

        SpectralSlices spectre;

        //0.1 seconds buffer = 4410 samples

        unsigned int startOffset = 0;
        for (unsigned int i = 0; i < slicesCount; i++) {
            const auto slice = samples.slice(startOffset, windowSize);
            const auto& result = fourierTransform->applyTo(slice);
            const auto magnitudes = extractMagnitude(result.slice(0, windowSize / 2).detach());
            spectre.slices.emplace_back(magnitudes);

            startOffset += overlapping;
        }
    }

    void SpectrogramComputer::init(size_t samplesSize) {
        slicesCount = ((samplesSize - windowSize) / overlapping) + 1;
        auto remainderSamples = (samplesSize - windowSize) % overlapping;
        remainder = std::make_unique<DoubleVector>(remainderSamples);
    }

    std::vector<double> SpectrogramComputer::extractMagnitude(const ComplexVector &transformResult) const {
        std::vector<double> ret;
        ret.reserve(windowSize / 2);
        for (const auto &complex: transformResult.constContent()) {
            auto magnitude = complex.magnitude();
            auto normalized = (magnitude < 0.1) ? 0.0 : 10 * std::log10(magnitude);

            ret.push_back(normalized);
        }

        return ret;
    }

}

