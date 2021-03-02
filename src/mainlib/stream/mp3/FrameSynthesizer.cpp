#include "FrameSynthesizer.h"

#include <mainlib/math/types.h>

#include <cmath>
#include <stdexcept>

using namespace aixa::math;

std::vector<unsigned int> FrameSynthesizer::pretab = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      1, 1, 1, 1, 2, 2, 3, 3, 3, 2, 0};

FrameSynthesizer::FrameSynthesizer() :
    antialiasCoefficients(), dequantized(),
    cosineTransformMatrix(NR_CODED_SAMPLES_PER_BAND * 2, NR_CODED_SAMPLES_PER_BAND) {
    initAntialiasCoefficients();
    initTransformMatrix();
    initBlockWindows();
}

void FrameSynthesizer::initAntialiasCoefficients() {
    const std::vector<double> ci = {-0.6, -0.535, -0.33, -0.185, -0.095, -0.041, -0.0142, -0.0037};

    for (int i = 0; i < NR_BUTTERFLIES; i++) {
        auto sq = std::sqrt(1.0 + ci[i] * ci[i]);
        antialiasCoefficients.cs[i] = 1.0 / sq;
        antialiasCoefficients.ca[i] = ci[i] / sq;
    }
}

void FrameSynthesizer::initTransformMatrix() {
    const auto MAX_FREQ = 2 * NR_TOTAL_SAMPLES;
    for (std::size_t i = 0; i < NR_TOTAL_SAMPLES; i++) {
        for (std::size_t j = 0; j < NR_CODED_SAMPLES_PER_BAND; j++) {
            auto freq = (2 * i + 1 + NR_CODED_SAMPLES_PER_BAND) * (2 * j + 1) % (4 * NR_TOTAL_SAMPLES);
            cosineTransformMatrix(j, i) = std::cos((M_PI * freq) / MAX_FREQ);
        }
    }
}

void FrameSynthesizer::initBlockWindows() {
    auto extendVector = [](const DoubleVector& v) -> DoubleMatrix {
        auto ret = DoubleMatrix(NR_TOTAL_SAMPLES, NR_TOTAL_SAMPLES);

        for (std::size_t row = 0; row < NR_TOTAL_SAMPLES; row++) {
            for (std::size_t col = 0; col < NR_TOTAL_SAMPLES; col++) {
                ret(row, col) = v[row];
            }
        }

        return ret;
    };

    auto v = DoubleVector(NR_TOTAL_SAMPLES, DoubleVector::Ordering::COLUMN_ORDERED);
    for (std::size_t i = 0; i < NR_TOTAL_SAMPLES; i++) {
        v[i] = std::sin(M_PI / NR_TOTAL_SAMPLES * (i + 0.5));
    }
    blockWindows.insert({GranuleChannelSideInfo::BlockType::NORMAL, std::move(extendVector(v))});

    v = DoubleVector(NR_TOTAL_SAMPLES, DoubleVector::Ordering::COLUMN_ORDERED);
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
    blockWindows.insert({GranuleChannelSideInfo::BlockType::START, std::move(extendVector(v))});

    v = DoubleVector(NR_TOTAL_SAMPLES, DoubleVector::Ordering::COLUMN_ORDERED);
    for (i = 0; i < 12; i++) {
        v[i] = std::sin(M_PI / 12 * (i + 0.5));
    }
    for (; i < NR_TOTAL_SAMPLES; i++) {
        v[i] = 0.0;
    }
    blockWindows.insert({GranuleChannelSideInfo::BlockType::THREE_SHORT, std::move(extendVector(v))});

    v = DoubleVector(NR_TOTAL_SAMPLES, DoubleVector::Ordering::COLUMN_ORDERED);
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
    blockWindows.insert({GranuleChannelSideInfo::BlockType::END, std::move(extendVector(v))});
}

void FrameSynthesizer::synthesize(unsigned int samplingFreq,
                                  const SideInformation& sideInfo,
                                  const MainDataContent& content,
                                  std::size_t nChannels) {
    for (unsigned int i = 0; i < NR_GRANULES; i++) {
        for (unsigned int channel = 0; channel < nChannels; channel++) {
            const auto& channelInfo = sideInfo.granules[i][channel];
            const auto& channelContent = content.granules[i][channel];
            dequantizeSamples(samplingFreq, channelInfo, channelContent);
            // reordering (short windows only)
            // stereo
            antialias(channelInfo);
            inverseMDCT(channelInfo);
        }
    }
}

void FrameSynthesizer::dequantizeSamples(unsigned int samplingFreq,
                                         const GranuleChannelSideInfo& channelInfo,
                                         const GranuleChannelContent& channelContent) {
    if (channelInfo.blockType == GranuleChannelSideInfo::BlockType::THREE_SHORT) {
        throw std::runtime_error("Not supported (yet) short windows");
    }

    double gainTerm = std::pow(2.0, (channelInfo.globalGain - GAIN_BASE) / 4.0);
    std::size_t scaleFactorBandInd = 1;
    std::size_t nextSubbandBoundary = samplingFreqBandIndexes[samplingFreq].longWindow[scaleFactorBandInd];

    for (std::size_t band = 0; band < NR_FREQ_BANDS; band++) {
        for (std::size_t sampleInd = 0; sampleInd < NR_CODED_SAMPLES_PER_BAND; sampleInd++) {
            if ((band * NR_CODED_SAMPLES_PER_BAND) + sampleInd == nextSubbandBoundary) {
                scaleFactorBandInd++;
                nextSubbandBoundary = samplingFreqBandIndexes[samplingFreq].longWindow[scaleFactorBandInd];
            }

            auto scaleFactor = channelContent.longWindowScaleFactorBands[scaleFactorBandInd - 1];
            double preTabFactor = channelInfo.preFlag * pretab[scaleFactorBandInd - 1];
            double exp = -0.5 * (1.0 + channelInfo.scaleFactorScale) * (scaleFactor + preTabFactor);
            double scaleFactorTerm = std::pow(2.0, exp);
            auto sample = channelContent.freqBands[band][sampleInd];
            auto dequantizedSample = std::pow(std::abs(sample), 4.0 / 3.0) * gainTerm * scaleFactorTerm;
            dequantizedSample *= (sample < 0) ? -1 : 1;
            dequantized[band][sampleInd] = dequantizedSample;
        }
    }
}

void FrameSynthesizer::antialias(const GranuleChannelSideInfo& channelInfo) {
    if (channelInfo.windowSwitching)
        throw std::runtime_error("Nope yet");

    for (std::size_t band = 0; band < NR_FREQ_BANDS - 1; band++) {
        for (std::size_t butterfly = 0; butterfly < NR_BUTTERFLIES; butterfly++) {
            auto upIndex = NR_CODED_SAMPLES_PER_BAND - 1 - butterfly;
            auto bandUp = dequantized[band][upIndex];
            auto bandDown = dequantized[band + 1][butterfly];
            dequantized[band][upIndex] =
                antialiasCoefficients.cs[butterfly] * bandUp - antialiasCoefficients.ca[butterfly] * bandDown;
            dequantized[band + 1][butterfly] =
                antialiasCoefficients.ca[butterfly] * bandUp + antialiasCoefficients.cs[butterfly] * bandDown;
        }
    }
}

void FrameSynthesizer::inverseMDCT(const GranuleChannelSideInfo& info) {
    /*
    *         N=36;
          for(p= 0;p<N;p++){
              sum = 0.0;
              for(m=0;m<N/2;m++)
                  sum += in[m] * COS[((2*p+1+N/2)*(2*m+1))%(4*36)];
              out[p] = sum * win[block_type][p];
          }
          */
    const auto& window = blockWindows.at(info.blockType);
    auto mulWindow = cosineTransformMatrix * window;
    for (auto& band : dequantized) {
        auto samplesMatrix = DoubleMatrix(NR_CODED_SAMPLES_PER_BAND, 1, band);
        auto timeDomainSamples = (samplesMatrix * mulWindow).row(0);
    }
}
