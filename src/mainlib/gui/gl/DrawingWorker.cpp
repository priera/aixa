#include "DrawingWorker.h"

#include <QtGui/QOffscreenSurface>
#include <chrono>
#include <thread>

#include "mainlib/gui/scene/Scene.h"

DrawingWorker::DrawingWorker(std::unique_ptr<QSurface> contextSurface, Scene &scene) :
    QThread(), scene(&scene), offscreenSurface(std::move(contextSurface)),
    frameRate(60),  // TODO make frameRate dependent on actual screen configuration
    context() {}

void DrawingWorker::run() {
    long int iterationTimeus = (1 / ((float)frameRate)) * 1000000;

    std::chrono::microseconds durationRender(iterationTimeus);
    auto ticksRender = durationRender.count();

    context->create();
    context->makeCurrent(offscreenSurface.get());

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
