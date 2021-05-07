#include "DrawWidget.h"

#include <QMimeData>

#include "utils.h"

DrawWidget::DrawWidget(Scene &scene) :
    QOpenGLWidget(), QOpenGLFunctions(), scene(&scene), centralNoteManager(nullptr),
    textureCollection(nullptr) {
    setAcceptDrops(true);
}

void DrawWidget::notifyNewValue(const Note &note) {
    if (centralNoteManager)
        centralNoteManager->setNewFrontNote(note);
}

void DrawWidget::renderNow() { update(); }

void DrawWidget::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, width(), height());

    emit initialized(context());

    glCheckError();
}

void DrawWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void DrawWidget::paintGL() {
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->draw();
    glCheckError();
}

void DrawWidget::dragEnterEvent(QDragEnterEvent *event) { event->acceptProposedAction(); }

void DrawWidget::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        emit urlDropped(mimeData->urls().first());
    }
    event->acceptProposedAction();
}

void DrawWidget::dragLeaveEvent(QDragLeaveEvent *event) { event->accept(); }
