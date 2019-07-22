#include "mainlib/gui/RenderableObject.h"

#include <QtGui/QOpenGLShaderProgram>
#include <iostream>

RenderableObject::RenderableObject(const QMatrix4x4 &projectionMatrix, QOpenGLShaderProgram &program)
 : projectionMatrix(projectionMatrix)
 , program(&program)
{
    initializeOpenGLFunctions();
}

RenderableObject::RenderableObject(const QMatrix4x4 &projectionMatrix)
: projectionMatrix(projectionMatrix)
, program(nullptr)
{
    initializeOpenGLFunctions();
}

RenderableObject::~RenderableObject() { }

void RenderableObject::addChildObject(float z, RenderableObject *object) {
    int zpos = z * 1000;
    children[zpos] = object;
}

void RenderableObject::render() {
    beforeRender();

    for (auto &child: children)
    {
        applyChildTransformations(child.second);

        child.second->render();
    }

    program->setUniformValue("model", modelMatrix);

    doMyRender();

    modelMatrix.setToIdentity();

    afterRender();
}


void RenderableObject::doMyRender() { }

void RenderableObject::beforeRender() { }

void RenderableObject::afterRender() { }

void RenderableObject::applyChildTransformations(RenderableObject *pObject) { }

void RenderableObject::moveCenterAt(float x, float y, float z) {
    modelMatrix.translate(x - w/2, y - h/2, z - d/2);
}

void RenderableObject::rotate(float degrees) {
    modelMatrix.rotate(degrees, 0.0f, 1.0f, 0.0f);
}


