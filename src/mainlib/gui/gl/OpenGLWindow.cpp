#include "OpenGLWindow.h"

#include <mainlib/globals.h>
#include <mainlib/gui/objects/ImmutableTextBox.h>
#include <mainlib/gui/objects/SpectrogramPlane.h>
#include <mainlib/gui/objects/TexturedPlane.h>
#include <mainlib/gui/objects/YScale.h>

#include <QtCore/QTimer>
#include <QtGui/QOpenGLContext>
#include <QtGui/QResizeEvent>
#include <QtGui/QScreen>

#include "GLContextManager.h"
#include "utils.h"

OpenGLWindow::OpenGLWindow(Scene &scene, std::unique_ptr<QOpenGLContext> &context,
                           BitmapBuilders &bitmapBuilders) :
    QWindow(),
    QOpenGLFunctions(),
    scene(&scene),
    context(std::move(context)),
    initialized(false),
    centralNoteManager(nullptr),
    bitmapBuilders(&bitmapBuilders),
    textureCollection(nullptr) {
    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow() { context->doneCurrent(); }

void OpenGLWindow::renderNow() {
    if (!isExposed()) return;

    if (!initialized) {
        init();
        initialized = true;
    }

    context->makeCurrent(this);

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->draw();

    glCheckError();

    context->swapBuffers(this);
    context->doneCurrent();
}

void OpenGLWindow::init() {
    context->setFormat(requestedFormat());
    if (!context->create()) {
        throw std::runtime_error("Error when creating OpenGLContext");
    }

    context->makeCurrent(this);
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
    glCheckError();

    context->doneCurrent();
}

bool OpenGLWindow::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::UpdateRequest:
            renderNow();
            return true;
        default:
            return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event) {
    Q_UNUSED(event);

    if (isExposed()) renderNow();
}

void OpenGLWindow::resizeEvent(QResizeEvent *ev) {
    if (!initialized) return;

    auto s = ev->size();
    glViewport(0, 0, s.width(), s.height());
}

void OpenGLWindow::notifyNewValue(const Note &note) {
    if (centralNoteManager) centralNoteManager->setNewFrontNote(note);
}
