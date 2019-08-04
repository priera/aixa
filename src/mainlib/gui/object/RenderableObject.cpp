#include "RenderableObject.h"

#include <QtGui/QOpenGLShaderProgram>
#include <iostream>

RenderableObject::RenderableObject(QOpenGLShaderProgram &program) :
  program(&program)
{
    initializeOpenGLFunctions();
}

RenderableObject::RenderableObject() :
 program(nullptr)
{
    initializeOpenGLFunctions();
}

RenderableObject::~RenderableObject() { }

void RenderableObject::addChildObject(float z, RenderableObject *object) {
    int zpos = z * 1000;
    children[zpos] = object;
}

void RenderableObject::update() {
    updateMatrix.setToIdentity();

    doMyUpdate();

    for (auto &child: children)
    {
        child.second->update();
        applyChildTransformations(child.second);
        child.second->updateDone();
    }

}

void RenderableObject::updateDone() {
    std::lock_guard<std::mutex> l(updateMutex);
    renderMatrix = updateMatrix;
}

void RenderableObject::render(QMatrix4x4 & projectionMatrix) {
    beforeRender(projectionMatrix);

    for (auto &child: children)
    {
        child.second->render(projectionMatrix);
    }

    std::unique_lock<std::mutex> l(updateMutex);
    program->setUniformValue("model", renderMatrix);

    doMyRender();

    l.unlock();

    afterRender();
}

float &RenderableObject::chooseParamForAnimation(const AnimationParam param) {
    static float dummy = 0;

    switch (param) {
        case AnimationParam::ANGLE:
            return angle;
            break;
        case AnimationParam::W:
            return w;
            break;
        case AnimationParam::H:
            return h;
            break;
        case AnimationParam::D:
            return d;
            break;
    }

    return dummy;
}

void RenderableObject::doMyUpdate() {
    auto t = std::chrono::steady_clock::now();

    for (auto &animation : animations) {
        if (animation.second.done())
            continue;

        animation.second.evaluate(t, chooseParamForAnimation(animation.first));
    }

    rotate(angle);
}

void RenderableObject::setupAnimation(AnimationParam param, std::chrono::milliseconds duration, unsigned int samples, \
        float startValue, float endValue, const Animation::HermiteParams & params) {
    animations[param].reset(duration, samples, startValue, endValue, params);
}

void RenderableObject::doMyRender() { }

void RenderableObject::beforeRender(const QMatrix4x4 & projectionMatrix) { }

void RenderableObject::afterRender() { }

void RenderableObject::applyChildTransformations(RenderableObject *pObject) { }

void RenderableObject::moveCenterAt(float x, float y, float z) {
    updateMatrix.translate(x - w / 2, y - h / 2, z - d / 2);
}

void RenderableObject::rotate(float degrees) {
    updateMatrix.rotate(degrees, 0.0f, 1.0f, 0.0f);
}
