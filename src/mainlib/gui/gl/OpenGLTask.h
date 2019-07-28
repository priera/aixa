#ifndef ALSAPLAYGROUND_OPENGLTASK_H
#define ALSAPLAYGROUND_OPENGLTASK_H

#include <QtCore/QThread>

#include <QtGui/QSurfaceFormat>


class QSurfaceFormat;
class QOpenGLContext;
class QOffscreenSurface;

class OpenGLWindow;

class CentralNoteManager;
class Scene;

class OpenGLTask : public QThread {
Q_OBJECT
public:
    explicit OpenGLTask(OpenGLWindow &win);
    virtual ~OpenGLTask();

    void run() override;
    void quit();

    CentralNoteManager *getCentralNoteManager() { return noteManager.get(); }
    Scene *getScene() { return scene.get(); }

signals:
    void renderLoopDone();
    void sceneBuilt();

private:
    int frameRate;

    std::unique_ptr<CentralNoteManager> noteManager;

    std::shared_ptr<QOpenGLContext> context;

    std::unique_ptr<Scene> scene;
    OpenGLWindow *window;

    std::atomic<bool> stop;
};


#endif //ALSAPLAYGROUND_OPENGLTASK_H
