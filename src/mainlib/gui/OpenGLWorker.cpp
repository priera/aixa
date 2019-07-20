#include "mainlib/gui/OpenGLWorker.h"

#include <chrono>
#include <cmath>
#include <iostream>

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QImage>
#include <QtGui/QMatrix4x4>

#include "mainlib/gui/RenderableObject.h"
#include "NoteProvider.h"

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
    this->object = nullptr;

    context->makeCurrent(surface);

    initializeOpenGLFunctions();

    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    projection.ortho(0.0f, w, 0.0f, h, -1, 1);
    projection.translate((float)w / 2, (float)h / 2);

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

    if (!object) {
        NoteProvider noteProvider(projection);
        object = noteProvider.generateNote('F');
    }

    if (object) object->render();

    ++m_frame;

    context->swapBuffers(surface);
}
