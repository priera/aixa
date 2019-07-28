#include "mainlib/gui/RenderableObject.h"

#include <QtGui/QOpenGLShaderProgram>

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
    doMyUpdate();

    for (auto &child: children)
    {
        applyChildTransformations(child.second);
    }
}

void RenderableObject::render(QMatrix4x4 & projectionMatrix) {
    beforeRender(projectionMatrix);

    for (auto &child: children)
    {
        child.second->render(projectionMatrix);
    }

    program->setUniformValue("model", modelMatrix);

    doMyRender();

    modelMatrix.setToIdentity();

    afterRender();
}

void RenderableObject::doMyUpdate() { }

void RenderableObject::doMyRender() { }

void RenderableObject::beforeRender(const QMatrix4x4 & projectionMatrix) { }

void RenderableObject::afterRender() { }

void RenderableObject::applyChildTransformations(RenderableObject *pObject) { }

void RenderableObject::moveCenterAt(float x, float y, float z) {
    modelMatrix.translate(x - w/2, y - h/2, z - d/2);
}

void RenderableObject::rotate(float degrees) {
    modelMatrix.rotate(degrees, 0.0f, 1.0f, 0.0f);
}
