#include "mainlib/gui/gl/GLContextManager.h"

#include <exception>
#include <iostream>

GLContextManager *GLContextManager::instance = nullptr;

GLContextManager &GLContextManager::getInstance() {
    if (!instance) {
        instance = new GLContextManager();
    }

    return *instance;
}

void GLContextManager::release() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

GLContextManager::GLContextManager() {
    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);

    offscreenSurface = new QOffscreenSurface();
    offscreenSurface->setFormat(format);
    offscreenSurface->create();

    sharedContext = new QOpenGLContext();
    sharedContext->setFormat(offscreenSurface->format());
    sharedContext->create();
    //sharedContext->makeCurrent(offscreenSurface);

    assert(glGetError() == GL_NO_ERROR);

    //sharedContext->doneCurrent();
}

QOpenGLContext *GLContextManager::createContext() {
    auto context = new QOpenGLContext();

    context->setShareContext(sharedContext);
    QSurfaceFormat sf = context->format();

    //sf.setOption(QSurfaceFormat::DebugContext);

    //context->setFormat(sf);

    /*if (!context->create()) {
        throw std::runtime_error("Error when creating OpenGLContext");
    } */

    return context;
}

/*QOpenGLContext *GLContextManager::useNewOffscreenContext() {
    auto ret = createContext();

    ret->makeCurrent(offscreenSurface);

    return ret;
} */