#include "DrawWidget.h"

#include <mainlib/globals.h>
#include <mainlib/gui/objects/SpectrogramPlane.h>
#include <mainlib/gui/objects/YScale.h>

#include <QMimeData>

#include "utils.h"

DrawWidget::DrawWidget(Scene &scene, BitmapBuilders &bitmapBuilders) :
    QOpenGLWidget(), QOpenGLFunctions(), scene(&scene), bitmapBuilders(&bitmapBuilders),
    centralNoteManager(nullptr), textureCollection(nullptr) {
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

    this->textureCollection = new TextureCollection(*bitmapBuilders);

    auto spectrogramTexture = this->textureCollection->buildSpectrogramTexture();
    auto spectrogramPlane = new SpectrogramPlane(bitmapBuilders->spectrogram(), spectrogramTexture);
    scene->addObject(std::shared_ptr<SpectrogramPlane>(spectrogramPlane));

    YScale *yScale_p;
    if (USE_LOG_SCALES) {
        yScale_p = YScale::buildLogarithmic(22050.0f, *textureCollection);
    } else {
        yScale_p = YScale::buildLinear(22050.0f, 10, *textureCollection);
    }

    auto yScale = std::shared_ptr<YScale>(yScale_p);
    scene->addObject(yScale);

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
