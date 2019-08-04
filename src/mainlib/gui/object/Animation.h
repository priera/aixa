#ifndef ALSAPLAYGROUND_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H
#define ALSAPLAYGROUND_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H

#include <chrono>

class RenderableObject;

class Animation {
public:
    Animation(std::chrono::milliseconds duration, unsigned int samples);
    virtual ~Animation();

    void evaluate(RenderableObject &object);
    bool done() const { return done_; }

private:
    unsigned int samples;

    bool done_;
    std::chrono::steady_clock::time_point begin;
    std::chrono::milliseconds timeBetweenSamples;

};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H
