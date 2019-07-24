#ifndef ALSAPLAYGROUND_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H

#include <memory>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOffscreenSurface>


class GLContextManager {
public:
    static GLContextManager &getInstance();
    static void release();

    QOpenGLContext *createContext();

    virtual ~GLContextManager();

private:
    GLContextManager();

    static GLContextManager instance;

    void init();

    bool initialized;
    QOpenGLContext *sharedContext;
    QOffscreenSurface *surface;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
