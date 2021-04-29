#include "GraphicsEnvironmentFactory.h"

#include <mainlib/gui/gl/DrawWidget.h>
#include <mainlib/gui/scene/SceneState.h>

#include <QOffscreenSurface>

std::unique_ptr<GraphicsEnvironment> GraphicsEnvironmentFactory::build(const QSize &appInitialSize) {
    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    auto offscreenSurface = std::make_unique<QOffscreenSurface>();
    offscreenSurface->setFormat(format);
    offscreenSurface->create();

    auto scene = std::make_unique<Scene>(appInitialSize.width(), appInitialSize.height());
    auto bitmapsProvider = std::make_unique<BitmapBuilders>();

    auto mainWidget = std::make_unique<DrawWidget>(*scene);

    auto drawingWorker = std::make_unique<DrawingWorker>(std::move(offscreenSurface), *scene);
    mainWidget->resize(appInitialSize.width(), appInitialSize.height());

    return std::make_unique<GraphicsEnvironment>(std::move(scene), std::move(drawingWorker),
                                                 std::move(mainWidget), std::move(bitmapsProvider));
}
