#include "DrawWidget.h"

#include <mainlib/globals.h>
#include <mainlib/gui/objects/SpectrogramPlane.h>
#include <mainlib/gui/objects/YScale.h>

#include <QEvent>

#include "utils.h"

DrawWidget::DrawWidget(Scene &scene, BitmapBuilders &bitmapBuilders) :
    QOpenGLWidget(), QOpenGLFunctions(), scene(&scene), bitmapBuilders(&bitmapBuilders),
    centralNoteManager(nullptr), textureCollection(nullptr) {}

void DrawWidget::notifyNewValue(const Note &note) {
    if (centralNoteManager)
        centralNoteManager->setNewFrontNote(note);
}

void DrawWidget::renderNow() {
    paintGL();
    update();
}

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

void DrawWidget::resizeGL(int w, int h) {
    this->makeCurrent();

    glViewport(0, 0, w, h);

    this->doneCurrent();
}

void DrawWidget::paintGL() {
    // According to Qt's documentation, following line shouldn't be necessary.
    // But bugs arise if this is not done explicitly here
    this->makeCurrent();

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->draw();
    glCheckError();

    this->doneCurrent();
}
