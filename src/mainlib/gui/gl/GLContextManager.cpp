#include "mainlib/gui/gl/GLContextManager.h"

#include <exception>
#include <iostream>
#include <sstream>

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

GLContextManager::GLContextManager() : contextId(1) {
    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setOption(QSurfaceFormat::DebugContext);

    offscreenSurface = std::make_unique<QOffscreenSurface>();
    offscreenSurface->setFormat(format);
    offscreenSurface->create();

    sharedContext = std::make_unique<QOpenGLContext>();
    sharedContext->setObjectName("shared OpenGL context");
    sharedContext->setFormat(offscreenSurface->format());
    sharedContext->create();

    assert(glGetError() == GL_NO_ERROR);
}

QOpenGLContext *GLContextManager::createContext() {
    std::stringstream s;
    s << "context" << contextId++;

    auto context = new QOpenGLContext();
    context->setObjectName(QString::fromStdString(s.str()));
    context->setShareContext(sharedContext.get());

    return context;
}
