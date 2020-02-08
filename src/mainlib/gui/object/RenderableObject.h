#ifndef ALSAPLAYGROUND_RENDERABLEOBJECT_H
#define ALSAPLAYGROUND_RENDERABLEOBJECT_H

#include <map>
#include <mutex>

#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QOpenGLShaderProgram>

#include <QtGui/QMatrix4x4>

#include "mainlib/gui/object/Animation.h"

class RenderableObject : protected QOpenGLExtraFunctions {
public:
    RenderableObject(QOpenGLShaderProgram &program);
    virtual ~RenderableObject();

    void update();
    void render(QMatrix4x4 & projectionMatrix);

    void moveCenterAt(float x, float y, float z);
    void rotate(float degrees);

    void addChildObject(float z, RenderableObject *object);

protected:
    enum class AnimationParam {
        W,
        H,
        D,
        ANGLE
    };

    RenderableObject();

    void setProgram(QOpenGLShaderProgram &program) {
        this->program = &program;
    }

    virtual void doMyUpdate();
    virtual void updateDone();

    virtual void doMyRender();
    virtual void beforeRender(const QMatrix4x4 & projectionMatrix);
    virtual void afterRender();

    virtual void applyChildTransformations(RenderableObject *pObject);

    void setupAnimation(AnimationParam param, std::chrono::milliseconds duration, unsigned int samples, \
        float startValue, float endValue, const Animation::HermiteParams & params);

    float w, h, d;
    float angle;

    std::map<int, RenderableObject *> children;

    QOpenGLShaderProgram *program;

private:
    float &chooseParamForAnimation(const AnimationParam param);

    QMatrix4x4 updateMatrix;
    QMatrix4x4 renderMatrix;

    std::mutex updateMutex;

    std::map<AnimationParam, Animation> animations;
};

#endif //ALSAPLAYGROUND_RENDERABLEOBJECT_H