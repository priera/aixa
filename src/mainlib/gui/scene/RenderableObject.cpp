#include "RenderableObject.h"

#include <QtGui/QOpenGLShaderProgram>

RenderableObject::RenderableObject(Dimensions dimensions) : program(nullptr), dim(dimensions) {
    initializeOpenGLFunctions();
    updateMatrix.setToIdentity();
}

RenderableObject::RenderableObject(QOpenGLShaderProgram &program, Dimensions dimensions) :
    program(&program), dim(dimensions) {
    initializeOpenGLFunctions();
    updateMatrix.setToIdentity();
}

void RenderableObject::addChildObject(float z, std::shared_ptr<RenderableObject> object) {
    int zpos = static_cast<int>(z * 1000);
    children[zpos] = std::move(object);
}

void RenderableObject::clearChildren() { children.clear(); }

void RenderableObject::update() {
    updateMatrix.setToIdentity();
    doMyUpdate();

    for (auto &child : children) {
        child.second->update();
        applyChildTransformations(*child.second);
        child.second->updateDone();
    }
}

void RenderableObject::updateDone() {
    std::lock_guard<std::mutex> l(updateMutex);
    renderMatrix = updateMatrix;
}

void RenderableObject::render(QMatrix4x4 &projectionMatrix) {
    if (!checkInitialized())
        return;

    beforeRender(projectionMatrix);

    for (auto &child : children) {
        child.second->render(projectionMatrix);
    }

    std::unique_lock<std::mutex> l(updateMutex);
    program->setUniformValue("model", renderMatrix);

    doMyRender();

    l.unlock();

    afterRender();
}

bool RenderableObject::checkInitialized() {
    if (!initialized) {
        if (!readyToInitialize()) {
            return false;
        } else {
            init();
            initialized = true;
        }
    }

    return true;
}

float &RenderableObject::chooseParamForAnimation(AnimationParam param) {
    switch (param) {
        case AnimationParam::ANGLE:
            return angle;
        case AnimationParam::W:
            return dim.width;
        case AnimationParam::H:
            return dim.height;
        case AnimationParam::D:
            return dim.depth;
        case AnimationParam::COLOR:
            return color;
        default:
            // This actually never will be thrown, but compilers complain less if this line is present
            throw std::runtime_error("invalid animation param");
    }
}

void RenderableObject::doMyUpdate() {
    auto t = std::chrono::steady_clock::now();

    for (auto &animation : animations) {
        if (animation.second.done())
            continue;

        animation.second.evaluate(t, chooseParamForAnimation(animation.first));
    }
}

void RenderableObject::setupAnimation(AnimationParam param,
                                      std::chrono::milliseconds duration,
                                      unsigned int samples,
                                      float startValue,
                                      float endValue,
                                      const Animation::HermiteParams &params,
                                      Animation::UpdateFunc updateFunc) {
    animations[param].reset(duration, samples, startValue, endValue, params, std::move(updateFunc));
}

void RenderableObject::doMyRender() {}

void RenderableObject::beforeRender(const QMatrix4x4 &projectionMatrix) {}

void RenderableObject::afterRender() {}

void RenderableObject::applyChildTransformations(RenderableObject &pObject) {}

void RenderableObject::moveCenterAt(float x, float y, float z) {
    updateMatrix.translate(x - dim.width / 2, y - dim.height / 2, z - dim.depth / 2);
}

void RenderableObject::rotate(float degrees) { updateMatrix.rotate(degrees, 0.0f, 1.0f, 0.0f); }

void RenderableObject::scale(float amount) { updateMatrix.scale(amount); }

bool RenderableObject::readyToInitialize() { return true; }

void RenderableObject::init() {}
