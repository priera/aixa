#include "GraphicsEnvironmentFactory.h"

#include <mainlib/gui/gl/DrawWidget.h>
#include <mainlib/gui/gl/GLContextManager.h>
#include <mainlib/gui/gl/OpenGLWindow.h>

std::unique_ptr<GraphicsEnvironment> GraphicsEnvironmentFactory::build(const QSize &appInitialSize) {
    auto scene = std::make_unique<Scene>(appInitialSize.width(), appInitialSize.height());
    auto bitmapsProvider = std::make_unique<BitmapBuilders>();

    GLContextManager::getInstance();
    auto mainWidget = std::make_unique<DrawWidget>(*scene, *bitmapsProvider);
    auto &surface = GLContextManager::getInstance().getOffscreenSurface();

    auto drawingWorker = std::make_unique<DrawingWorker>(surface, *scene);
    mainWidget->resize(appInitialSize.width(), appInitialSize.height());

    return std::make_unique<GraphicsEnvironment>(std::move(scene), std::move(drawingWorker),
                                                 std::move(mainWidget), std::move(bitmapsProvider));
}
