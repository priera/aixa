#ifndef AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H
#define AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H

#include <QGenericMatrix>
#include <chrono>

class RenderableObject;

class Animation {
public:
    using UpdateFunc = std::function<void(float)>;
    using AnimationClock = std::chrono::steady_clock;

    // In order to test these parameters, use webs like https://cubic-bezier.com
    struct HermiteParams {
        float m0x, m0y, m1x, m1y;
    };

    struct Params {
        std::chrono::milliseconds duration;
        unsigned int samples;
        float startValue;
        float endValue;
        HermiteParams hParams;
        UpdateFunc update;
    };

    explicit Animation(Params&& params);
    virtual ~Animation() = default;

    void evaluate(const AnimationClock::time_point& at, float v);
    bool done() const noexcept { return done_; }

private:
    static QGenericMatrix<4, 4, float> buildCoefficients();

    static constexpr float GRADIENT_SCALE = 3.3;

    unsigned int samples;
    float startValue;
    float endValue;
    UpdateFunc updateFunction;

    const QGenericMatrix<4, 4, float> HermiteCoefficients;
    bool done_;
    bool evaluatedOnce;

    QGenericMatrix<2, 4, float> hermiteParams;

    std::chrono::steady_clock::time_point begin;
    std::chrono::milliseconds timeBetweenSamples;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H
