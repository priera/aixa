#ifndef AIXA_SRC_MAINLIB_GUI_SCENE_ANIMATIONLIST_H
#define AIXA_SRC_MAINLIB_GUI_SCENE_ANIMATIONLIST_H

#include <chrono>
#include <cstddef>

#include "Animation.h"

class AnimationList {
public:
    virtual ~AnimationList() = default;

    void add(Animation::Params&& params) { animations.emplace_back(std::forward<Animation::Params>(params)); }

    void evaluate(const Animation::AnimationClock::time_point& at, float v);

    bool done() const noexcept { return currentIndex == animations.size(); }

private:
    std::size_t currentIndex{0};
    std::vector<Animation> animations;
};

#endif  // AIXA_SRC_MAINLIB_GUI_SCENE_ANIMATIONLIST_H
