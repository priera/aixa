#include "SpectrogramComputer.h"

#include <mainlib/math/ConstVectorProxy.h>

#include "SpectrogramFragment.h"

namespace aixa::math {

    void SpectrogramComputer::computeOn(const DoubleVector &samples) {
        if (!initialized()) {
            init(samples.size());
        } // else: adjust indexes and offsets

        SpectrogramFragment fragment(slicesCount);
        unsigned int startOffset = 0;

        for (unsigned int i = 0; i < slicesCount; i++) {
            const auto slice = samples.copy(startOffset, windowSize) * mask;
            const auto& result = fourierTransform->applyTo(slice);
            const auto relevantResult = result.slice(0, sliceSize).detach();
            storeMagnitude(relevantResult, fragment.slices[i]);

            startOffset += overlapping;
        }

        sendToReceiver(std::move(fragment));
    }

    void SpectrogramComputer::init(size_t samplesSize) {
        slicesCount = ((samplesSize - windowSize) / overlapping) + 1;
        auto remainderSamples = (samplesSize - windowSize) % overlapping;
        remainder = std::make_unique<DoubleVector>(remainderSamples);
    }

    void SpectrogramComputer::storeMagnitude(const ComplexVector &result, std::vector<double> &storeAt) const {
        const auto &content = result.constContent();
        for (std::size_t i = 0; i < sliceSize; i++) {
            auto magnitude = content[i].magnitude();
            auto normalized = (magnitude < 1.0) ? 0.0 : 10 * std::log10(magnitude);

            scale->insert(i, normalized);
        }

        scale->moveDataTo(storeAt);
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

