#include "mainlib/gui/OpenGLWindow.h"

#include <QtGui/QScreen>
#include <QtGui/QOpenGLContext>

#include "mainlib/gui/OpenGLWorker.h"


OpenGLWindow::OpenGLWindow(std::unique_ptr<QOpenGLContext> &context) :
    QWindow(),
    QOpenGLFunctions(),
    context(std::move(context)),
    initialized(false)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow() {
    context->doneCurrent();
}

void OpenGLWindow::render()
{
    if (!initialized) {
        init();
        initialized = true;
    }

    glClearColor(0.f, 1.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //actual rendering here

    context->swapBuffers(this);
}

void OpenGLWindow::init() {
    this->context->makeCurrent(this);

    auto con = QOpenGLContext::currentContext();

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::UpdateRequest:
            render();
            return true;
        default:
            return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        render();
}
