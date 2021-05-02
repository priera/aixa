#include "Animation.h"

#include <cassert>
#include <iostream>

Animation::Animation(Animation::Params &&params) :
    samples(params.samples), startValue(params.startValue), endValue(params.endValue),
    updateFunction(std::move(params.update)), HermiteCoefficients(buildCoefficients()), done_(false),
    evaluatedOnce(false) {
    timeBetweenSamples = params.duration / params.samples;

    float allParams[] = {0.0,
                         0.0,
                         1.0,
                         1.0,
                         params.hParams.m0x * GRADIENT_SCALE,
                         params.hParams.m0y * GRADIENT_SCALE,
                         params.hParams.m1x * GRADIENT_SCALE,
                         params.hParams.m1y * GRADIENT_SCALE};

    hermiteParams = QGenericMatrix<2, 4, float>(allParams);
}

QGenericMatrix<4, 4, float> Animation::buildCoefficients() {
    float values[16] = {1, 0, 0, 0, 0, 0, 1, 0, -3, 3, -2, -1, 2, -2, 1, 1};

    return QGenericMatrix<4, 4, float>(values);
}

void Animation::evaluate(const AnimationClock::time_point &at, float v) {
    if (done_)
        return;

    if (!evaluatedOnce) {
        begin = std::chrono::steady_clock::now();
        evaluatedOnce = true;
    }

    float sample = (at - begin) / timeBetweenSamples;

    if (sample > samples) {
        done_ = true;
        return;
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

    updateFunction(v);
}
