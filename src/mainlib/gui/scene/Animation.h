#ifndef AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H
#define AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H

#include <QGenericMatrix>
#include <chrono>

class RenderableObject;

class Animation {
public:
    using UpdateFunc = std::function<void(float)>;

    // In order to test these parameters, use webs like https://cubic-bezier.com
    struct HermiteParams {
        float m0x, m0y, m1x, m1y;
    };

    Animation();
    virtual ~Animation();

    void reset(std::chrono::milliseconds duration,
               unsigned int samples,
               float startValue,
               float endValue,
               const HermiteParams &params,
               UpdateFunc update);

    void evaluate(const std::chrono::steady_clock::time_point &at, float &v);
    bool done() const { return done_; }

private:
    static QGenericMatrix<4, 4, float> buildCoefficients();

    const QGenericMatrix<4, 4, float> HermiteCoefficients;

    static constexpr float GRADIENT_SCALE = 3.3;

    unsigned int samples;
    float startValue;
    float endValue;
    UpdateFunc updateFunction;

    bool done_;
    QGenericMatrix<2, 4, float> hermiteParams;

    std::chrono::steady_clock::time_point begin;
    std::chrono::milliseconds timeBetweenSamples;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H
