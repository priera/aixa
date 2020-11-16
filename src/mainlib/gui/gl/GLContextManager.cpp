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

    offscreenSurface = std::make_unique<QOffscreenSurface>();
    offscreenSurface->setFormat(format);
    offscreenSurface->create();

    sharedContext = std::make_unique<QOpenGLContext>();
    sharedContext->setFormat(offscreenSurface->format());
    sharedContext->create();

    assert(glGetError() == GL_NO_ERROR);
}

QOpenGLContext *GLContextManager::createContext() {
    auto context = new QOpenGLContext();

    context->setShareContext(sharedContext.get());
    QSurfaceFormat sf = context->format();

    //sf.setOption(QSurfaceFormat::DebugContext);

    return context;
}
