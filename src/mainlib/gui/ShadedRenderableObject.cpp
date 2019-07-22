#include "ShadedRenderableObject.h"

#include <iostream>

ShadedRenderableObject::ShadedRenderableObject(const QMatrix4x4 &projectionMatrix, const QString &vertexShaderPath, const QString &fragmentShaderPath) :
    RenderableObject(projectionMatrix) {
    programContainer = std::make_unique<QOpenGLShaderProgram>();

    auto program = programContainer.get();

    if (vertexShaderPath != "") {
        program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath);
    }

    if (fragmentShaderPath != "") {
        program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath);
    }

    program->link();
    std::cout << program->log().toStdString() << std::endl;

    setProgram(*program);

    program->bind();
    program->setUniformValue("projection", projectionMatrix);
    program->release();
}

void ShadedRenderableObject::beforeRender() {
    program->bind();
}

void ShadedRenderableObject::afterRender() {
    program->release();
}