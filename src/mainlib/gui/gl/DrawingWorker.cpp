#include "DrawingWorker.h"

#include <chrono>
#include <thread>

#include <QtGui/QOffscreenSurface>

#include "OpenGLWindow.h"
#include "mainlib/gui/CentralNoteManager.h"

#include "mainlib/gui/gl/Scene.h"
#include <mainlib/gui/gl/GLContextManager.h>

#include <iostream>

DrawingWorker::DrawingWorker(std::unique_ptr<QOpenGLContext> &context,
                             QSurface &contextSurface,
                             Scene &scene) :
    QThread(),
    frameRate(60), //TODO make this dependent on actual screen configuration
    context(std::move(context)),
    scene(&scene),
    offscreenSurface(&contextSurface),
    centralNoteManager(nullptr) {

}

void DrawingWorker::notifyNewValue(const Note &note) {
    if (centralNoteManager)
        centralNoteManager->setNewFrontNote(note);
}


void DrawingWorker::run() {
    if (!context->create()) {
        throw std::runtime_error("Error when creating OpenGLContext");
    }
    context->makeCurrent(offscreenSurface);

    centralNoteManager = std::make_unique<CentralNoteManager>();
    scene->setMainObject(centralNoteManager.get());

    long int iterationTimeus = (1 / ((float)frameRate)) * 1000000;

    std::chrono::microseconds durationRender(iterationTimeus);
    auto ticksRender = durationRender.count();

    m_stop = false;
    while (!m_stop) {
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
