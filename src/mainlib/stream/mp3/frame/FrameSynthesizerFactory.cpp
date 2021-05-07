#include "FrameSynthesizerFactory.h"

#include <fstream>

#include "LongWindowAlgorithms.h"
#include "ShortWindowAlgorithms.h"

using namespace aixa::math;

FrameSynthesizer* FrameSynthesizerFactory::build() const {
    auto antialiasCoefficients = computeAntialiasCoefficients();
    auto longWindowTransform = computeLongWindowTransformMatrix();
    auto shortWindowTransform = computeShortWindowTransformMatrix();
    auto blockWindows = generateBlockWindows();
    auto frequencyInversion = computeFrequencyInversionMatrix();
    auto synFilter = computeTimeDomainSynFilter();
    auto dWindowMatrix = parseDWindowMatrix();

    auto longWindowAlgorithms = std::make_unique<LongWindowAlgorithms>(longWindowTransform);
    auto shortWindowAlgorithms = std::make_unique<ShortWindowAlgorithms>(shortWindowTransform);

    return new FrameSynthesizer(std::move(longWindowAlgorithms), std::move(shortWindowAlgorithms),
                                antialiasCoefficients, blockWindows, frequencyInversion, synFilter,
                                dWindowMatrix);
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

aixa::math::DoubleMatrix FrameSynthesizerFactory::computeLongWindowTransformMatrix() const {
    auto ret = aixa::math::DoubleMatrix(NR_TOTAL_SAMPLES, NR_CODED_SAMPLES_PER_BAND);

    const auto MAX_FREQ = 2 * NR_TOTAL_SAMPLES;
    for (std::size_t i = 0; i < NR_TOTAL_SAMPLES; i++) {
        for (std::size_t j = 0; j < NR_CODED_SAMPLES_PER_BAND; j++) {
            auto freq = static_cast<double>((2 * i + 1 + NR_CODED_SAMPLES_PER_BAND) * (2 * j + 1) %
                                            (4 * NR_TOTAL_SAMPLES));
            ret(j, i) = std::cos((M_PI * freq) / MAX_FREQ);
        }
    }

    return ret;
}

aixa::math::DoubleMatrix FrameSynthesizerFactory::computeShortWindowTransformMatrix() const {
    auto ret = aixa::math::DoubleMatrix(NR_SHORT_WINDOW_BANDS, NR_SHORT_WINDOW_BAND_SAMPLES);

    const auto MAX_FREQ = 2 * NR_SHORT_WINDOW_BANDS;
    for (std::size_t i = 0; i < NR_SHORT_WINDOW_BANDS; i++) {
        for (std::size_t j = 0; j < NR_SHORT_WINDOW_BAND_SAMPLES; j++) {
            auto freq = static_cast<double>((2 * i + 1 + NR_SHORT_WINDOW_BAND_SAMPLES) * (2 * j + 1));
            ret(j, i) = std::cos((M_PI * freq) / MAX_FREQ);
        }
    }

    return ret;
}

FrameSynthesizer::BlockWindows FrameSynthesizerFactory::generateBlockWindows() const {
    auto ret = FrameSynthesizer::BlockWindows();

    auto diagonalize = [](const DoubleVector& v) -> DoubleMatrix {
        auto dim = v.rows();
        auto ret = DoubleMatrix(dim, dim);

        for (std::size_t n = 0; n < dim; n++) {
            ret(n, n) = v[n];
        }

        return ret;
    };

    auto v = DoubleVector(NR_TOTAL_SAMPLES);
    for (std::size_t i = 0; i < NR_TOTAL_SAMPLES; i++) {
        v[i] = std::sin(M_PI / NR_TOTAL_SAMPLES * (i + 0.5));
    }
    ret.insert({GranuleChannelSideInfo::BlockType::NORMAL, std::move(diagonalize(v))});

    v = DoubleVector(NR_TOTAL_SAMPLES);
    std::size_t i;
    for (i = 0; i < 18; i++) {
        v[i] = std::sin(M_PI / NR_TOTAL_SAMPLES * (i + 0.5));
    }
    for (; i < 24; i++) {
        v[i] = 1.0;
    }
    for (; i < 30; i++) {
        v[i] = std::sin(M_PI / 12 * (i + 0.5 - 18));
    }
    for (; i < NR_TOTAL_SAMPLES; i++) {
        v[i] = 0.0;
    }
    ret.insert({GranuleChannelSideInfo::BlockType::START, std::move(diagonalize(v))});

    v = DoubleVector(NR_SHORT_WINDOW_BANDS);
    for (i = 0; i < NR_SHORT_WINDOW_BANDS; i++) {
        v[i] = std::sin(M_PI / NR_SHORT_WINDOW_BANDS * (i + 0.5));
    }
    ret.insert({GranuleChannelSideInfo::BlockType::THREE_SHORT, std::move(diagonalize(v))});

    v = DoubleVector(NR_TOTAL_SAMPLES);
    for (i = 0; i < 6; i++) {
        v[i] = 0.0;
    }
    for (; i < 12; i++) {
        v[i] = std::sin(M_PI / 12 * (i + 0.5 - 6));
    }
    for (; i < 18; i++) {
        v[i] = 1.0;
    }
    for (; i < NR_TOTAL_SAMPLES; i++) {
        v[i] = std::sin(M_PI / 36 * (i + 0.5));
    }
    ret.insert({GranuleChannelSideInfo::BlockType::END, std::move(diagonalize(v))});

    return ret;
}

aixa::math::DoubleMatrix FrameSynthesizerFactory::computeFrequencyInversionMatrix() const {
    auto ret = DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, NR_FREQ_BANDS);
    for (std::size_t row = 0; row < ret.rows(); row++) {
        for (std::size_t col = 0; col < ret.columns(); col++) {
            auto val = (row % 2 == 1 && col % 2 == 1) ? -1.0 : 1.0;
            ret(row, col) = val;
        }
    }

    return ret;
}

aixa::math::DoubleMatrix FrameSynthesizerFactory::computeTimeDomainSynFilter() const {
    auto ret = DoubleMatrix(FrameSynthesizer::NR_D_WINDOW_MATRIXED_VECTOR_SIZE, NR_FREQ_BANDS);
    for (std::size_t col = 0; col < ret.columns(); col++) {
        for (std::size_t row = 0; row < ret.rows(); row++) {
            auto val = 1e9 * std::cos(((M_PI / 64) * col + (M_PI / 4)) * (2.0 * row + 1));
            val += (val >= 0.0) ? 0.5 : -0.5;
            std::modf(val, &val);
            val *= 1e-9;
            ret(row, col) = val;
        }
    }

    return ret;
}

aixa::math::DoubleMatrix FrameSynthesizerFactory::parseDWindowMatrix() const {
    auto f = std::ifstream(dWindowPath);
    if (!f) {
        throw std::runtime_error("Could not find dWindowFile at " + dWindowPath.string());
    }

    auto ret = DoubleMatrix(FrameSynthesizer::NR_FIFO_SIZE, FrameSynthesizer::NR_D_WINDOW_VECTOR_SIZE);
    for (std::size_t col = 0; col < FrameSynthesizer::NR_FIFO_SIZE; col++) {
        for (std::size_t row = 0; row < FrameSynthesizer::NR_D_WINDOW_VECTOR_SIZE; row++) {
            double d;
            f >> d;
            ret(row, col) = d;
        }
    }

    return ret;
}
