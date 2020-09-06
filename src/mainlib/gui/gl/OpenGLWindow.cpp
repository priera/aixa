#include "OpenGLWindow.h"

#include <QtGui/QOpenGLContext>
#include <QtGui/QResizeEvent>
#include <QtGui/QScreen>

#include "mainlib/gui/gl/GLContextManager.h"
#include "mainlib/gui/gl/Scene.h"
#include "mainlib/gui/gl/utils.h"

OpenGLWindow::OpenGLWindow(Scene &scene, std::unique_ptr<QOpenGLContext> &context)
        : QWindow(), QOpenGLFunctions(),
            scene(&scene),
            context(std::move(context)),
            initialized(false) {
    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow() { context->doneCurrent(); }

void OpenGLWindow::renderNow() {
    if (!initialized) {
        init();
        initialized = true;
    }

    context->makeCurrent(this);

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->draw();

    glCheckError();

    context->swapBuffers(this);

    context->doneCurrent();
}

void OpenGLWindow::init() {
    context->setFormat(requestedFormat());
    if (!context->create()) {
        throw std::runtime_error("Error when creating OpenGLContext");
    }

    context->makeCurrent(this);

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glViewport(0, 0, width(), height());

    context->doneCurrent();
}

bool OpenGLWindow::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::UpdateRequest:
            renderNow();
            return true;
        default:
            return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event) {
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::resizeEvent(QResizeEvent *ev) {
    if (!initialized)
        return;

    auto s = ev->size();
    glViewport(0, 0, s.width(), s.height());
}
