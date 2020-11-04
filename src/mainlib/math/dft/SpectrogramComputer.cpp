#include "SpectrogramComputer.h"

#include <mainlib/math/ConstVectorProxy.h>

#include "SpectrogramFragment.h"

namespace aixa::math {

    void SpectrogramComputer::computeOn(const DoubleVector &samples) {
        if (!initialized()) {
            init(samples.size());
        } // else: adjust indexes and offsets

        SpectrogramFragment fragment;

        //0.1 seconds buffer = 4410 samples

        unsigned int startOffset = 0;
        for (unsigned int i = 0; i < slicesCount; i++) {
            const auto slice = samples.copy(startOffset, windowSize) * mask;
            const auto& result = fourierTransform->applyTo(slice);
            const auto magnitudes = extractMagnitude(result.slice(0, windowSize / 2).detach());
            fragment.slices.emplace_back(magnitudes);

            startOffset += overlapping;
        }

        sendToReceiver(std::move(fragment));
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
            auto normalized = (magnitude < 1.0) ? 0.0 : 10 * std::log10(magnitude);

            if (normalized < 0.0) {
                char a = 3;
            }

            ret.push_back(normalized);
        }

        return ret;
    }

    void SpectrogramComputer::buildMask() {
        mask[0] = 0.0;
        mask[(windowSize / 2) - 1] = 1.0;
        mask[windowSize / 2] = 1.0;
        mask[windowSize - 1] = 0.0;

        for (std::size_t i = 1; i < (windowSize / 2) - 1; i++) {
            auto value = (static_cast<double>(2 * i) / windowSize);
            mask[i] = value;
            mask[windowSize - 1 - i ] = value;
        }
    }

}

