#include "mainlib/gui/OpenGLWorker.h"

#include <chrono>
#include <cmath>
#include <iostream>

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QImage>
#include <QtGui/QMatrix4x4>

#include "mainlib/gui/RenderableObject.h"
#include "mainlib/gui/CentralNoteManager.h"

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

    //glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    projection.perspective(45.0f,  (float) w / (float)h, 0.1f, 100.0f);
    QMatrix4x4 view;
    view.lookAt({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    projection *= view;

    glViewport(0, 0, w, h);
}

void OpenGLWorker::setSize(int w, int h) {
    this->w = w;
    this->h = h;

    glViewport(0, 0, w, h);
}

void OpenGLWorker::draw() {
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    centralNoteManager->render();

    ++m_frame;

    context->swapBuffers(surface);
}
