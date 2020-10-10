#include "GraphicsEnvironmentFactory.h"

#include <mainlib/gui/gl/GLContextManager.h>
#include <mainlib/gui/gl/OpenGLWindow.h>

std::unique_ptr<GraphicsEnvironment> GraphicsEnvironmentFactory::build(const QSize &appInitialSize) {
    auto scene = std::make_unique<Scene>(appInitialSize.width(), appInitialSize.height());

    auto context_p = GLContextManager::getInstance().createContext();
    auto context = std::unique_ptr<QOpenGLContext>(context_p);
    auto &surface = GLContextManager::getInstance().getOffscreenSurface();

    auto drawingWorker = std::make_unique<DrawingWorker>(context, surface, *scene);
    context_p->moveToThread(drawingWorker.get());

    context_p = GLContextManager::getInstance().createContext();
    context = std::unique_ptr<QOpenGLContext>(context_p);

    auto openGlWindow = std::make_unique<OpenGLWindow>(*scene, context);
    openGlWindow->resize(appInitialSize.width(), appInitialSize.height());

    QObject::connect(drawingWorker.get(), &DrawingWorker::computeLoopDone,
                     openGlWindow.get(), &OpenGLWindow::renderNow,
                     Qt::QueuedConnection);

    return std::make_unique<GraphicsEnvironment>(scene, drawingWorker, openGlWindow);
}
