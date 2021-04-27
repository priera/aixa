#include "DrawingWorker.h"

#include <QtGui/QOffscreenSurface>
#include <chrono>
#include <thread>

#include "GLContextManager.h"
#include "OpenGLWindow.h"
#include "mainlib/gui/scene/Scene.h"

DrawingWorker::DrawingWorker(QSurface &contextSurface, Scene &scene) :
    QThread(), frameRate(60),  // TODO make this dependent on actual screen configuration
    context(), scene(&scene), offscreenSurface(&contextSurface) {
    // this->context->moveToThread(this);
}

void DrawingWorker::run() {
    long int iterationTimeus = (1 / ((float)frameRate)) * 1000000;

    std::chrono::microseconds durationRender(iterationTimeus);
    auto ticksRender = durationRender.count();

    context->create();
    context->makeCurrent(offscreenSurface);

    m_stop = false;
    while (!m_stop) {
        auto start = std::chrono::steady_clock::now();

        scene->update();

        emit computeLoopDone();

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
