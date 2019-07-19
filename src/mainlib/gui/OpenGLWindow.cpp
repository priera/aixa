#include "mainlib/gui/OpenGLWindow.h"

#include <QtGui/QScreen>

#include "mainlib/gui/OpenGLWorker.h"


OpenGLWindow::OpenGLWindow(OpenGLWorker &worker, QOpenGLContext *context) :
    QOpenGLWindow(context),
    worker(&worker) {}

void OpenGLWindow::initializeGL() {
    emit readyGL();
}

void OpenGLWindow::paintGL() {
    //Called always after a resize
    const qreal retinaScale = devicePixelRatio();
    int w = width() * retinaScale;
    int h = height() * retinaScale;

    worker->setSize(w, h);
}

void OpenGLWindow::resizeGL(int w, int h) {
    setWidth(w);
    setHeight(h);
}
