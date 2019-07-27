#ifndef ALSAPLAYGROUND_OPENGLTASK_H
#define ALSAPLAYGROUND_OPENGLTASK_H

#include <mutex>
#include <memory>
#include <atomic>
#include <thread>
#include <condition_variable>

#include <QtCore/QThread>

#include <QtGui/QSurfaceFormat>


class QSurfaceFormat;
class QOpenGLContext;
class QOffscreenSurface;

class OpenGLWindow;

class OpenGLWorker;
class CentralNoteManager;

class OpenGLTask : public QThread {
Q_OBJECT
public:
    explicit OpenGLTask(OpenGLWindow &win);
    virtual ~OpenGLTask();

    void run() override;

    CentralNoteManager *getCentralNoteManager() { return noteManager.get(); }

signals:
    void renderLoopDone();

private:
    void initWorkerThreadObjects();

    void buildWindow();

    void runningLoop();

    QSurfaceFormat format;

    int frameRate;

    std::unique_ptr<CentralNoteManager> noteManager;

    std::unique_ptr<OpenGLWorker> worker;
    std::shared_ptr<QOpenGLContext> context;

    bool contextCreated, windowCreated;

    std::mutex contextMutex, windowMutex;
    std::condition_variable cvContext, cvWindow;

    OpenGLWindow *window;
};


#endif //ALSAPLAYGROUND_OPENGLTASK_H
