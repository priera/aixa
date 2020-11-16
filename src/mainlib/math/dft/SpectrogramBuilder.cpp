#include "SpectrogramBuilder.h"

#include <mainlib/math/scale/LinearScale.h>
#include <mainlib/math/scale/LogScale.h>

#include "FourierTransformFactory.h"

namespace aixa::math {
    SpectrogramComputer *SpectrogramBuilder::build() const {
        auto transform_p = getFourierTransformFactory(fourierImplementation).build(transformSize);
        auto transform = std::unique_ptr<FourierTransform>(transform_p);
        auto scale = std::unique_ptr<Scale>(buildScale(*transform));
        auto spectrogramComputer = new SpectrogramComputer(std::move(transform), std::move(scale));

        return spectrogramComputer;
    }

    Scale *SpectrogramBuilder::buildScale(const FourierTransform &fourierTransform) const {
        if (logScale) {
            return new LogScale(fourierTransform.baseContinuousFreq(samplePeriod),
                                fourierTransform.maxContinuousFreq(samplePeriod),
                                fourierTransform.relevantSize());
        } else {
            return new LinearScale(fourierTransform.relevantSize());
        }
    }
}
