#include "AnimationList.h"

void AnimationList::evaluate(const Animation::AnimationClock::time_point& at, float v) {
    if (currentIndex < animations.size()) {
        auto& current = animations[currentIndex];
        if (current.done()) {
            currentIndex++;
            evaluate(at, v);
        } else {
            current.evaluate(at, v);
        }
    }
}
