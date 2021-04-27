#ifndef AIXA_OPENGLTASK_H
#define AIXA_OPENGLTASK_H

#include <mainlib/gui/scene/Scene.h>

#include <QtCore/QThread>
#include <QtGui/QSurface>
#include <QtGui/QSurfaceFormat>

class DrawingWorker : public QThread {
    Q_OBJECT
public:
    explicit DrawingWorker(QSurface &contextSurface, Scene &scene);

    void run() override;

    void stop() { m_stop = true; }

    void setContext(QOpenGLContext *ctx) {
        context = std::unique_ptr<QOpenGLContext>(ctx);
        context->moveToThread(this);
    }

signals:
    void computeLoopDone();

private:
    int frameRate;

    std::unique_ptr<QOpenGLContext> context;
    Scene *scene;
    QSurface *offscreenSurface;

    std::atomic<bool> m_stop;
};

#endif  // AIXA_OPENGLTASK_H
