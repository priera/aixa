#include "FrameSynthesizerFactory.h"

FrameSynthesizer* FrameSynthesizerFactory::build() const {
    auto antialiasCoefficients = computeAntialiasCoefficients();
    auto cosineTransform = computeTransformMatrix();

    return new FrameSynthesizer(antialiasCoefficients, cosineTransform);
}

FrameSynthesizer::AntialiasCoefficients FrameSynthesizerFactory::computeAntialiasCoefficients() const {
    auto ret = FrameSynthesizer::AntialiasCoefficients();
    const std::vector<double> ci = {-0.6, -0.535, -0.33, -0.185, -0.095, -0.041, -0.0142, -0.0037};

    for (int i = 0; i < FrameSynthesizer::NR_BUTTERFLIES; i++) {
        auto sq = std::sqrt(1.0 + ci[i] * ci[i]);
        ret.cs[i] = 1.0 / sq;
        ret.ca[i] = ci[i] / sq;
    }

    return ret;
}

aixa::math::DoubleMatrix FrameSynthesizerFactory::computeTransformMatrix() const {
    auto ret = aixa::math::DoubleMatrix(NR_CODED_SAMPLES_PER_BAND * 2, NR_CODED_SAMPLES_PER_BAND);

    const auto MAX_FREQ = 2 * NR_TOTAL_SAMPLES;
    for (std::size_t i = 0; i < NR_TOTAL_SAMPLES; i++) {
        for (std::size_t j = 0; j < NR_CODED_SAMPLES_PER_BAND; j++) {
            auto freq = (2 * i + 1 + NR_CODED_SAMPLES_PER_BAND) * (2 * j + 1) % (4 * NR_TOTAL_SAMPLES);
            ret(j, i) = std::cos((M_PI * freq) / MAX_FREQ);
        }
    }

    return ret;
}
