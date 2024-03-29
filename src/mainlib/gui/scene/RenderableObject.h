#ifndef AIXA_RENDERABLEOBJECT_H
#define AIXA_RENDERABLEOBJECT_H

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <map>
#include <memory>
#include <mutex>

#include "Animation.h"
#include "AnimationList.h"
#include "Dimensions.h"

class RenderableObject : protected QOpenGLExtraFunctions {
public:
    explicit RenderableObject(Dimensions dimensions);
    explicit RenderableObject(QOpenGLShaderProgram &program, Dimensions dimensions);

    virtual ~RenderableObject() = default;

    bool isInitialized() const { return initialized; }

    void update();
    void render(QMatrix4x4 &projectionMatrix);

    void moveCenterAt(float x, float y, float z);
    void rotate(float degrees);
    void scale(float amount);

    void addChildObject(float z, std::shared_ptr<RenderableObject> object);
    void clearChildren();

    float getColor() const noexcept { return color; }

protected:
    enum class AnimationParam
    {
        W,
        H,
        D,
        ANGLE,
        COLOR
    };

    void setProgram(QOpenGLShaderProgram &program) { this->program = &program; }
    void setColor(float color) noexcept { this->color = color; }

    virtual bool readyToInitialize();
    virtual void init();

    virtual void doMyUpdate();
    virtual void updateDone();

    virtual void doMyRender();
    virtual void beforeRender(const QMatrix4x4 &projectionMatrix);
    virtual void afterRender();

    virtual void applyChildTransformations(RenderableObject &pObject);

    void addAnimation(AnimationParam animable, Animation::Params &&animationParams) {
        animations[animable].add(std::forward<Animation::Params>(animationParams));
    }

    Dimensions dim;
    float angle{0};
    float color{0};

    std::map<int, std::shared_ptr<RenderableObject>> children;

    QOpenGLShaderProgram *program;

private:
    float &chooseParamForAnimation(AnimationParam param);
    bool checkInitialized();

    QMatrix4x4 updateMatrix;
    QMatrix4x4 renderMatrix;

    std::mutex updateMutex;

    std::map<AnimationParam, AnimationList> animations;
    bool initialized{false};
};

#endif  // AIXA_RENDERABLEOBJECT_H
