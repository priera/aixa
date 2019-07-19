#include "mainlib/gui/OpenGLWorker.h"

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QImage>
#include <QtGui/QMatrix4x4>

#include <chrono>
#include <cmath>
#include <iostream>
#include <QtGui/QOpenGLFunctions_3_3_Core>


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

    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    projection.ortho(0.0f, w, 0.0f, h, -1, 1);

    noteDrawer.init();

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

    static int currentNote = 0;

    std::vector<char> chars = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

    noteDrawer.drawNote(chars[currentNote], projection);

    if (m_frame % 120 == 0) {
        currentNote = (++currentNote) % chars.size();
    }

    ++m_frame;

    context->swapBuffers(surface);
}
