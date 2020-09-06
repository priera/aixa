#ifndef AIXA_OPENGLTASK_H
#define AIXA_OPENGLTASK_H

#include <QtCore/QThread>

#include <QtGui/QSurfaceFormat>
#include <QtGui/QSurface>

#include <mainlib/audio/note/Note.h>
#include <mainlib/gui/CentralNoteManager.h>


class QSurfaceFormat;
class QOpenGLContext;
class QOffscreenSurface;
class OpenGLWindow;
class Scene;

class DrawingWorker : public QThread, public NotesListener {
Q_OBJECT
public:
    //explicit DrawingWorker(OpenGLWindow &win);
    explicit DrawingWorker(std::unique_ptr<QOpenGLContext> &context,
                           QSurface &contextSurface,
                           Scene &scene);

    void run() override;

    void stop();

    void notifyNewValue(const Note& note) override;

    //CentralNoteManager *getCentralNoteManager() { return noteManager.get(); }
    //Scene *getScene() { return scene; }

signals:

    void renderLoopDone();

    //void sceneBuilt();

private:
    int frameRate;

    //std::unique_ptr<CentralNoteManager> noteManager;

    std::unique_ptr<QOpenGLContext> context;
    Scene *scene;
    QSurface *offscreenSurface;
    //OpenGLWindow *window;
    std::unique_ptr<CentralNoteManager> centralNoteManager;

    std::atomic<bool> m_stop;
};


#endif //AIXA_OPENGLTASK_H
