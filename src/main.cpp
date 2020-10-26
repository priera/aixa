
//#include "examples/AlsaExamples.h"

#include <QApplication>

#include <mainlib/audio/AudioWorkerFactory.h>
#include <mainlib/audio/note/NoteSetter.h>

#include <mainlib/gui/MainEventFilter.h>
#include <mainlib/gui/GraphicsEnvironmentFactory.h>
#include <mainlib/math/dft/FourierTransformFactory.h>

using namespace std::chrono_literals;
using namespace aixa::math;

//static const auto STREAM = "/home/pedro/alsaTests/amics.wav";
static const auto STREAM = "??";

std::unique_ptr<SpectrogramComputer> buildSpectrogramComputer(std::shared_ptr<SpectrogramConsumer> spectrogramConsumer) {
    auto impl = FourierTransformFactory::Implementations::FFT;
    auto transform = std::unique_ptr<FourierTransform>(getFourierTransformFactory(impl).build(256));
    auto spectrogramComputer = std::make_unique<SpectrogramComputer>(std::move(transform));
    spectrogramComputer->setReceiver(spectrogramConsumer);

    return spectrogramComputer;
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const auto w = static_cast<int>(1920 * 3.0 / 4);
    const auto h = static_cast<int>(1080 * 3.0 / 4);
    const auto appSize = QSize(w, h);

    auto graphicsEnvironment = GraphicsEnvironmentFactory::build(appSize);

    NoteSetter noteSetter;
    noteSetter.addObserver(graphicsEnvironment->getNotesListener());

    auto spectrogramComputer = buildSpectrogramComputer(graphicsEnvironment->getSpectrogramConsumer());
    auto audioWorker = AudioWorkerFactory(std::move(spectrogramComputer)).buildWithInputStream(STREAM);

    auto commandCollection = audioWorker->getCommandCollection();

    MainEventFilter mainEventFilter(commandCollection, noteSetter);
    app.installEventFilter(&mainEventFilter);

    graphicsEnvironment->start();
    audioWorker->start();

    int ret = app.exec();

    audioWorker->stop();
    graphicsEnvironment->stop();

    return ret;
}

