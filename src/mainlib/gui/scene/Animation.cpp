#include "Animation.h"

#include <cassert>

#include <iostream>

Animation::Animation() :
    HermiteCoefficients(buildCoefficients()),
    done_(true) { }

Animation::~Animation() {}

QGenericMatrix<4, 4, float> Animation::buildCoefficients() {
    float values[16] = {
            1, 0, 0, 0,
            0, 0, 1, 0,
            -3, 3, -2, -1,
            2, -2, 1, 1
    };

    return QGenericMatrix<4, 4, float>(values);
}

void Animation::reset(std::chrono::milliseconds duration, unsigned int samples,  float startValue, float endValue, const HermiteParams & hmP) {
    assert(duration.count() > 0);
    assert(samples >= 2);

    this->samples = samples;
    this->startValue = startValue;
    this->endValue = endValue;

    timeBetweenSamples = duration / samples;

    float allParams[] = {
            0.0, 0.0,
            1.0, 1.0,
            hmP.m0x * GRADIENT_SCALE, hmP.m0y * GRADIENT_SCALE,
            hmP.m1x * GRADIENT_SCALE, hmP.m1y * GRADIENT_SCALE
    };

    hermiteParams = QGenericMatrix<2, 4, float>(allParams);

    begin = std::chrono::steady_clock::now();
    done_ = false;
}

bool Animation::evaluate(const std::chrono::steady_clock::time_point &at, float &v) {
    if (done_) return false;

    float sample = (at - begin) / timeBetweenSamples;

    if (sample > samples) {
        done_ = true;
        return false;
    }

    QGenericMatrix<4, 1, float> p;
    p(0, 0) = 1.0;

    sample = sample / samples;

    for (int i = 1; i < 4; i++) {
        p(0, i) = p(0, i - 1) * sample;
    }

    auto eval = (p * HermiteCoefficients) * hermiteParams;

    auto normalizedV = eval(0, 1);
    v = startValue + normalizedV * (endValue - startValue);

    return true;
}