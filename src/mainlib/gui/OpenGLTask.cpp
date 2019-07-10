#include "mainlib/gui/OpenGLTask.h"

#include <QtGui/QOpenGLWindow>

#include "mainlib/gui/OpenGLWorker.h"
#include "mainlib/gui/OpenGLWindow.h"

OpenGLTask::OpenGLTask(const QSurfaceFormat &format) :
    format(format),
    running(false),
    runningThread(nullptr)
{}

OpenGLTask::~OpenGLTask() {}

void OpenGLTask::start() {
    if (running) return;

    running = true;
    contextCreated = false;
    windowCreated = false;

    runningThread = std::make_unique<std::thread>([this](){ runningLoop(); });

    //build the window on the main Qt event loop
    buildWindow();
}

void OpenGLTask::stop() {
    if (!running) return;

    running = false;
    runningThread->join();

    runningThread = nullptr;
    context = nullptr;
}

void OpenGLTask::runningLoop() {
    initWorkerThreadObjects();

    while (running) {
        worker->draw();
    }
}

void OpenGLTask::initWorkerThreadObjects() {
    context = std::make_unique<QOpenGLContext>();
    context->setFormat(format);
    context->create();

    worker = std::make_unique<OpenGLWorker>(*context.get());

    {
        std::lock_guard<std::mutex> lk(contextMutex);
        contextCreated = true;
    }

    cvContext.notify_one();

    std::unique_lock<std::mutex> lk(windowMutex);
    cvWindow.wait(lk, [this]{return windowCreated; });

    const qreal retinaScale = window->devicePixelRatio();
    int w = window->width() * retinaScale;
    int h = window->height() * retinaScale;
    worker->bindToSurface(window.get(), w, h);
}

void OpenGLTask::buildWindow() {
    std::unique_lock<std::mutex> lk(contextMutex);
    cvContext.wait(lk, [this]{return contextCreated; });

    window = std::make_unique<OpenGLWindow>(*worker.get(), context.get());
    window->setFormat(format);
    window->resize(1920 / 3, 1080 / 3);

    QObject::connect(window.get(), &OpenGLWindow::readyGL, [this](){
        {
            std::lock_guard<std::mutex> lk(windowMutex);
            windowCreated = true;
        }

        cvWindow.notify_one();
    });

}