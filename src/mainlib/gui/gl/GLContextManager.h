#ifndef AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
#define AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H

#include <memory>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOffscreenSurface>

class GLContextManager {
public:
    static GLContextManager &getInstance();
    static void release();

    QOpenGLContext *createContext();
    QSurface &getOffscreenSurface() { return *offscreenSurface; }

private:
    GLContextManager();

    static GLContextManager *instance;

    std::unique_ptr<QOpenGLContext> sharedContext;
    std::unique_ptr<QOffscreenSurface> offscreenSurface;
};


#endif //AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
