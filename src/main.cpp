
//#include "examples/AlsaExamples.h"

#include <QApplication>

#include <mainlib/audio/AudioWorkerFactory.h>
#include <mainlib/audio/note/NoteSetter.h>

#include <mainlib/gui/MainEventFilter.h>
#include <mainlib/gui/GraphicsEnvironmentFactory.h>

using namespace std::chrono_literals;

//static const auto STREAM = "/home/pedro/alsaTests/amics.wav";
static const auto STREAM = "??";

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const auto w = static_cast<int>(1920 * 3.0 / 4);
    const auto h = static_cast<int>(1080 * 3.0 / 4);
    const auto appSize = QSize(w, h);

    auto graphicsEnvironment = GraphicsEnvironmentFactory::build(appSize);
    graphicsEnvironment->start();

    NoteSetter noteSetter;
    noteSetter.addObserver(graphicsEnvironment->getNotesListener());

    auto audioWorker = AudioWorkerFactory().buildWithInputStream(STREAM);
    audioWorker->start();

    auto commandCollection = audioWorker->getCommandCollection();

    MainEventFilter mainEventFilter(commandCollection, noteSetter);
    app.installEventFilter(&mainEventFilter);

    int ret = app.exec();

    audioWorker->stop();
    graphicsEnvironment->stop();

    return ret;
}

