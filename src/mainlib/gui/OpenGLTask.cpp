#include "mainlib/gui/OpenGLTask.h"

#include <chrono>
#include <thread>

#include <QtGui/QOpenGLWindow>
#include <iostream>

#include "mainlib/gui/OpenGLWorker.h"
#include "mainlib/gui/OpenGLWindow.h"

OpenGLTask::OpenGLTask(const QSurfaceFormat &format) :
    format(format),
    frameRate(60), //TODO make this dependent on actual screen configuration
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

    long int iterationTimeus = (1 / ((float)frameRate)) * 1000000;

    std::chrono::microseconds durationRender(iterationTimeus);
    auto ticksRender = durationRender.count();

    while (running) {
        auto start = std::chrono::steady_clock::now();

        worker->draw();

        auto end = std::chrono::steady_clock::now();
        auto renderTimeus = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        auto timeToWait = ticksRender - renderTimeus;

        if (timeToWait > 0) {
            std::chrono::microseconds us(timeToWait);
            std::this_thread::sleep_for(us);
        }

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
    //window->resize(1920 / 3, 1080 / 3);
    window->resize(800, 600);

    QObject::connect(window.get(), &OpenGLWindow::readyGL, [this](){
        {
            std::lock_guard<std::mutex> lk(windowMutex);
            windowCreated = true;
        }

        cvWindow.notify_one();
    });

}