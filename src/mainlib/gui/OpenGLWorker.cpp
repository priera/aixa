#include "mainlib/gui/OpenGLWorker.h"

#include <QtGui/QOpenGLShaderProgram>

#include <iostream>
#include <chrono>
#include <cmath>

OpenGLWorker::OpenGLWorker(QOpenGLContext &context) :
    QOpenGLExtraFunctions(),
    context(&context),
    surface(nullptr),
    w(0),
    h(0) {
}

OpenGLWorker::~OpenGLWorker() { }

void OpenGLWorker::bindToSurface(QSurface *surface, int w, int h) {
    this->surface = surface;
    this->w = w;
    this->h = h;

    context->makeCurrent(surface);

    initializeOpenGLFunctions();

    program = std::make_unique<QOpenGLShaderProgram>();
    
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "./src/mainlib/gui/vertex.glsl");
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "./src/mainlib/gui/fragment.glsl");

    program->link();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    vertices = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glViewport(0, 0, w, h);
}

void OpenGLWorker::setSize(int w, int h) {
    this->w = w;
    this->h = h;

    glViewport(0, 0, w, h);
}


void OpenGLWorker::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program->bind();

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    program->release();

    ++m_frame;

    context->swapBuffers(surface);
}