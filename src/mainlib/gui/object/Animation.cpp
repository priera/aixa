#include "mainlib/gui/object/Animation.h"

#include <cassert>

#include <iostream>

Animation::Animation(std::chrono::milliseconds duration, unsigned int samples) :
    samples(samples),
    done_(false) {
    assert(duration.count() > 0);
    assert(samples >= 2);

    timeBetweenSamples = duration / samples;

    std::cout << timeBetweenSamples.count() << std::endl;

    begin = std::chrono::steady_clock::now();
}

Animation::~Animation() {}

void Animation::evaluate(RenderableObject &object) {
    if (done_) return;

    auto now = std::chrono::steady_clock::now();
    unsigned int sample = (now - begin) / timeBetweenSamples;

    if (sample > samples) {
        done_ = true;
        return;
    }

}