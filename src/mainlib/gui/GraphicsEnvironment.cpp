#include "GraphicsEnvironment.h"

#include <mainlib/audio/AudioWorkerFactory.h>

GraphicsEnvironment::GraphicsEnvironment(std::unique_ptr<Scene> scene,
                                         std::unique_ptr<DrawingWorker> drawingWorker,
                                         std::unique_ptr<DrawWidget> mainWin,
                                         std::unique_ptr<BitmapBuilders> bitmapsProvider) :
    QObject(),
    scene(std::move(scene)), drawingWorker(std::move(drawingWorker)), mainWindow(std::move(mainWin)),
    bitmapsProvider(std::move(bitmapsProvider)), textureCollection(), sceneState() {
    connect(mainWindow.get(), &DrawWidget::initialized, this, &GraphicsEnvironment::setupUsingContext);
    connect(mainWindow.get(), &DrawWidget::urlDropped, this, &GraphicsEnvironment::checkProposedStream);
    connect(this->drawingWorker.get(), &DrawingWorker::computeLoopDone, this->mainWindow.get(),
            &DrawWidget::renderNow, Qt::QueuedConnection);
}

void GraphicsEnvironment::setupUsingContext(QOpenGLContext* guiContext) {
    this->textureCollection = std::make_unique<TextureCollection>(*bitmapsProvider);
    this->sceneState = std::make_unique<SceneState>(*scene, *textureCollection, *bitmapsProvider);
    this->sceneState->showUXControls();

    auto context = new QOpenGLContext();
    context->setShareContext(guiContext->shareContext());
    drawingWorker->setContext(context);
    drawingWorker->start();
}

void GraphicsEnvironment::checkProposedStream(const QUrl& url) {
    if (!url.isLocalFile())
        return;

    auto path = url.path();
    const auto& extension = path.split(".").last().toLower();
    if (!AudioWorkerFactory::acceptedFormats().contains(extension))
        return;

#ifdef WIN32
    // Fix for a bug which seems be present only on Windows
    if (path.startsWith('/')) {
        path = path.right(path.size() - 1);
    }
#endif
    auto stdPath = std::filesystem::path(path.toStdString());
    if (!exists(stdPath))
        return;

    emit streamReceived(stdPath);
}

void GraphicsEnvironment::showAudioVisualizations() {
    bitmapsProvider->resetSpectrogram();
    this->sceneState->showAudioVisualizations();
}
