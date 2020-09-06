
//#include "examples/AlsaExamples.h"

#include <QApplication>

#include <mainlib/audio/AudioWorkerFactory.h>

#include <mainlib/gui/MainEventFilter.h>

#include <mainlib/gui/gl/DrawingWorker.h>
#include <mainlib/gui/gl/OpenGLWindow.h>
#include <mainlib/gui/gl/GLContextManager.h>
#include <mainlib/gui/gl/Scene.h>

#include <mainlib/audio/note/NoteSetter.h>

using namespace std::chrono_literals;

//static const auto STREAM = "/home/pedro/alsaTests/amics.wav";
static const auto STREAM = "??";

static OpenGLWindow *buildOpenGLWindow(const QSize size, Scene &scene) {
    auto context_p = GLContextManager::getInstance().createContext();
    auto context = std::unique_ptr<QOpenGLContext>(context_p);

    auto ret = new OpenGLWindow(scene, context);
    ret->resize(size.width(), size.height());
    ret->show();

    return ret;
}

static DrawingWorker *buildDrawingWorker(const QSize &size, Scene &scene) {
    auto context_p = GLContextManager::getInstance().createContext();
    auto context = std::unique_ptr<QOpenGLContext>(context_p);
    auto &surface = GLContextManager::getInstance().getOffscreenSurface();

    auto ret = new DrawingWorker(context, surface, scene);
    context_p->moveToThread(ret);

    return ret;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const auto w = static_cast<int>(1920 * 3.0 / 4);
    const auto h = static_cast<int>(1080 * 3.0 / 4);
    const auto appSize = QSize(w, h);

    auto scene = std::make_unique<Scene>(appSize.width(), appSize.height());
    auto drawingWorker = std::unique_ptr<DrawingWorker>(buildDrawingWorker(appSize, *scene));
    auto win = std::unique_ptr<OpenGLWindow>(buildOpenGLWindow(appSize, *scene));
    QObject::connect(drawingWorker.get(), &DrawingWorker::renderLoopDone,
                     win.get(), &OpenGLWindow::renderNow,
                     Qt::QueuedConnection);

    NoteSetter noteSetter;
    noteSetter.addObserver(drawingWorker.get());

    auto audioWorker = AudioWorkerFactory().buildWithInputStream(STREAM);
    audioWorker->start();

    auto commandCollection = audioWorker->getCommandCollection();

    MainEventFilter mainEventFilter(commandCollection, noteSetter);
    app.installEventFilter(&mainEventFilter);

    drawingWorker->start();

    int ret = app.exec();

    audioWorker->stop();
    drawingWorker->stop();

    GLContextManager::release();

    return ret;
}

