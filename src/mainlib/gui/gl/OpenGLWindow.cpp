#include "OpenGLWindow.h"

#include <QtGui/QScreen>
#include <QtGui/QOpenGLContext>
#include <QtGui/QResizeEvent>
#include <iostream>

#include "mainlib/gui/gl/Scene.h"
#include "GLContextManager.h"

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

OpenGLWindow::OpenGLWindow() :
    QWindow(),
    QOpenGLFunctions(),
    context(nullptr),
    scene(nullptr),
    initialized(false),
    ready(false)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow() {
    context->doneCurrent();
}

void OpenGLWindow::render()
{
    if (scene) {
        scene->draw();
    }
}

void OpenGLWindow::renderNow() {
    if (!ready) return;

    if (!initialized) {
        init();
        initialized = true;
    }

    context->makeCurrent(this);

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render();

    glCheckError();
    //assert(glGetError() == GL_NO_ERROR);

    context->swapBuffers(this);

    context->doneCurrent();
}

void OpenGLWindow::init() {
    auto context_p = GLContextManager::getInstance().createContext();
    context = std::unique_ptr<QOpenGLContext>(context_p);

    context->setFormat(requestedFormat());

    context->makeCurrent(this);

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    glViewport(0, 0, width(), height());

    context->doneCurrent();
}

void OpenGLWindow::setReady() {
    ready = true;
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::UpdateRequest:
            renderNow();
            return true;
        default:
            return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::resizeEvent(QResizeEvent *ev) {
    if (!initialized) return;

    auto s = ev->size();
    glViewport(0, 0, s.width(), s.height());
}