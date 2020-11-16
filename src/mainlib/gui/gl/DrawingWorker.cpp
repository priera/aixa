#include "DrawingWorker.h"

#include <chrono>
#include <thread>

#include <QtGui/QOffscreenSurface>

#include "OpenGLWindow.h"
#include "mainlib/gui/scene/Scene.h"
#include "GLContextManager.h"

DrawingWorker::DrawingWorker(std::unique_ptr<QOpenGLContext> &context,
                             QSurface &contextSurface,
                             Scene &scene) :
    QThread(),
    frameRate(60), //TODO make this dependent on actual screen configuration
    context(std::move(context)),
    scene(&scene),
    offscreenSurface(&contextSurface) {

}

void DrawingWorker::run() {
    long int iterationTimeus = (1 / ((float)frameRate)) * 1000000;

    std::chrono::microseconds durationRender(iterationTimeus);
    auto ticksRender = durationRender.count();

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
