#ifndef ALSAPLAYGROUND_OPENGLTASK_H
#define ALSAPLAYGROUND_OPENGLTASK_H

#include <mutex>
#include <memory>
#include <atomic>
#include <thread>
#include <condition_variable>

#include <QtGui/QSurfaceFormat>

class QSurfaceFormat;
class QOpenGLContext;
class OpenGLWindow;

class OpenGLWorker;

class OpenGLTask {
public:
    explicit OpenGLTask(const QSurfaceFormat &format);
    virtual ~OpenGLTask();

    void start();
    void stop();

    OpenGLWindow *getWindow() { return window.get(); }

private:
    void initWorkerThreadObjects();

    void buildWindow();

    void runningLoop();

    QSurfaceFormat format;

    std::atomic<bool> running;
    std::unique_ptr<OpenGLWorker> worker;
    std::unique_ptr<std::thread> runningThread;
    std::unique_ptr<QOpenGLContext> context;

    bool contextCreated, windowCreated;

    std::mutex contextMutex, windowMutex;
    std::condition_variable cvContext, cvWindow;

    std::unique_ptr<OpenGLWindow> window;
};


#endif //ALSAPLAYGROUND_OPENGLTASK_H
