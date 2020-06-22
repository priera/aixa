#ifndef AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
#define AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H

#include <memory>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOffscreenSurface>


class GLContextManager {
public:
    static GLContextManager &getInstance();
    void release();

    QOpenGLContext *createContext();
    QOpenGLContext *useNewOffscreenContext();

    virtual ~GLContextManager();

private:
    GLContextManager();

    static GLContextManager instance;

    void init();

    bool initialized;
    QOpenGLContext *sharedContext;
    QOffscreenSurface *surface;
};


#endif //AIXA_SRC_MAINLIB_GUI_GL_GLCONTEXTMANAGER_H
