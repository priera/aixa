#include "mainlib/gui/RenderableObject.h"

#include <QtGui/QOpenGLShaderProgram>
#include <iostream>

RenderableObject::RenderableObject(const QMatrix4x4 &projectionMatrix, const QString &vertexShaderPath, const QString &fragmentShaderPath)
 : projectionMatrix(projectionMatrix)
{
    initializeOpenGLFunctions();

    program = std::make_unique<QOpenGLShaderProgram>();

    if (vertexShaderPath != "") {
        program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath);
    }

    if (fragmentShaderPath != "") {
        program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath);
    }

    program->link();
    std::cout << program->log().toStdString() << std::endl;

    program->bind();
    program->setUniformValue("projection", projectionMatrix);
    program->release();

}

RenderableObject::~RenderableObject() { }

void RenderableObject::render() {
    program->bind();

    program->setUniformValue("model", modelMatrix);

    doMyRender();

    program->release();

    modelMatrix.setToIdentity();
}

void RenderableObject::moveCenterAt(float x, float y) {
    modelMatrix.translate(x - w/2, y - h/2);
}

void RenderableObject::rotate(float degrees) {
    modelMatrix.rotate(degrees, 0.0f, 1.0f, 0.0f);
}