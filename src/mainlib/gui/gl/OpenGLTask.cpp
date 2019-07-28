#include "OpenGLTask.h"

#include <chrono>
#include <thread>

#include <QtGui/QOffscreenSurface>

#include "OpenGLWindow.h"
#include "mainlib/gui/CentralNoteManager.h"

#include "mainlib/gui/gl/Scene.h"
#include <mainlib/gui/gl/GLContextManager.h>

#include <iostream>

OpenGLTask::OpenGLTask(OpenGLWindow &win) :
    QThread(),
    window(&win),
    frameRate(60) //TODO make this dependent on actual screen configuration
{
    connect(this, &OpenGLTask::renderLoopDone, window, &OpenGLWindow::renderNow);
}

OpenGLTask::~OpenGLTask() {}

void OpenGLTask::run() {

    auto size = window->size();

    auto context = GLContextManager::getInstance().useNewOffscreenContext();

    noteManager = std::make_unique<CentralNoteManager>();
    scene = std::make_unique<Scene>(*noteManager, size.width(), size.height());

    emit sceneBuilt();

    long int iterationTimeus = (1 / ((float)frameRate)) * 1000000;

    std::chrono::microseconds durationRender(iterationTimeus);
    auto ticksRender = durationRender.count();

    stop = false;

    while (!stop) {
        auto start = std::chrono::steady_clock::now();

        scene->update();

        emit renderLoopDone();

        auto end = std::chrono::steady_clock::now();
        auto renderTimeus = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        auto timeToWait = ticksRender - renderTimeus;

        if (timeToWait > 0) {
            std::chrono::microseconds us(timeToWait);
            std::this_thread::sleep_for(us);
        }
    }

    context->doneCurrent();
}

void OpenGLTask::quit() {
    stop = true;
}
