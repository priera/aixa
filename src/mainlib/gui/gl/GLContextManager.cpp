#include "mainlib/gui/gl/GLContextManager.h"

#include <exception>

GLContextManager GLContextManager::instance;

GLContextManager::~GLContextManager() { }

GLContextManager &GLContextManager::getInstance() {
    if (!instance.initialized) {
        //Wait until the program is actually running to do the initialization
        instance.init();

        instance.initialized = true;
    }

    return instance;
}

void GLContextManager::release() {
    if (instance.initialized) {

        if (instance.surface) {
            delete instance.surface;
            instance.surface = nullptr;
        }

        if (instance.sharedContext) {
            delete instance.sharedContext;
            instance.sharedContext = nullptr;
        }
    }
}

GLContextManager::GLContextManager() :
    initialized(false) { }

void GLContextManager::init() {
    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);

    surface = new QOffscreenSurface();
    surface->setFormat(format);
    surface->create();

    sharedContext = new QOpenGLContext();
    sharedContext->setFormat(surface->format());
    sharedContext->create();
    sharedContext->makeCurrent(surface);

    assert(glGetError() == GL_NO_ERROR);

    sharedContext->doneCurrent();
}

QOpenGLContext *GLContextManager::createContext() {
    auto context = new QOpenGLContext();

    context->setShareContext(sharedContext);
    QSurfaceFormat sf = context->format();

    sf.setOption(QSurfaceFormat::DebugContext);

    context->setFormat(sf);
    if (!context->create()) {
        throw std::runtime_error("Error when creating OpenGLContext");
    }

    return context;
}
