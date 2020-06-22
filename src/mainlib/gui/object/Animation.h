#ifndef AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H
#define AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H

#include <chrono>

#include <QGenericMatrix>

class RenderableObject;

class Animation {
public:
    struct HermiteParams {
        float m0x, m0y, m1x, m1y;
    };

    Animation();
    virtual ~Animation();

    void reset(std::chrono::milliseconds duration, unsigned int samples, float startValue, float endValue, const HermiteParams & params);

    bool evaluate(const std::chrono::steady_clock::time_point &at, float &v);
    bool done() const { return done_; }

private:
    static QGenericMatrix<4, 4, float> buildCoefficients();

    const QGenericMatrix<4, 4, float> HermiteCoefficients;
    QGenericMatrix<2, 4, float> hermiteParams;

    static constexpr float GRADIENT_SCALE = 3.3;

    unsigned int samples;

    bool done_;

    float startValue;
    float endValue;

    std::chrono::steady_clock::time_point begin;
    std::chrono::milliseconds timeBetweenSamples;

};


#endif //AIXA_SRC_MAINLIB_GUI_OBJECT_ANIMATION_H
