#include "ShadedRenderableObject.h"

#include <iostream>

ShadedRenderableObject::ShadedRenderableObject(const QString &vertexShaderPath, const QString &fragmentShaderPath) :
    RenderableObject() {
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
}

void ShadedRenderableObject::beforeRender(const QMatrix4x4 & projectionMatrix) {
    program->bind();

    program->setUniformValue("projection", projectionMatrix);
}

void ShadedRenderableObject::afterRender() {
    program->release();
}