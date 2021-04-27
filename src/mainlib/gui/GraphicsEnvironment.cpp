#include "GraphicsEnvironment.h"

GraphicsEnvironment::GraphicsEnvironment(std::unique_ptr<Scene> scene,
                                         std::unique_ptr<DrawingWorker> drawingWorker,
                                         std::unique_ptr<DrawWidget> mainWindow,
                                         std::unique_ptr<BitmapBuilders> bitmapsProvider) :
    QObject(),
    scene(std::move(scene)), drawingWorker(std::move(drawingWorker)), mainWindow(std::move(mainWindow)),
    bitmapsProvider(std::move(bitmapsProvider)) {
    connect(this->mainWindow.get(), &DrawWidget::initialized, this, &GraphicsEnvironment::startWorker);
    connect(this->drawingWorker.get(), &DrawingWorker::computeLoopDone, this->mainWindow.get(),
            &DrawWidget::renderNow, Qt::QueuedConnection);
}

void GraphicsEnvironment::startWorker(QOpenGLContext* sharedContext) {
    auto context = new QOpenGLContext();
    context->setShareContext(sharedContext);
    drawingWorker->setContext(context);
    drawingWorker->start();
}
