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
    //QOpenGLContext *useNewOffscreenContext();
    QSurface &getOffscreenSurface() { return *offscreenSurface; }

private:
    GLContextManager();

    static GLContextManager *instance;

    QOpenGLContext *sharedContext;
    QOffscreenSurface *offscreenSurface;
};


#endif //AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
