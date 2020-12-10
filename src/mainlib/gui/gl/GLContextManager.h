#ifndef AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
#define AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H

#include <QtGui/QOffscreenSurface>
#include <QtGui/QOpenGLContext>
#include <memory>

class GLContextManager {
public:
    static GLContextManager &getInstance();
    static void release();

    QOpenGLContext *createContext();
    QSurface &getOffscreenSurface() { return *offscreenSurface; }

private:
    GLContextManager();

    static GLContextManager *instance;

    unsigned int contextId;

    std::unique_ptr<QOpenGLContext> sharedContext;
    std::unique_ptr<QOffscreenSurface> offscreenSurface;
};

#endif  // AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
