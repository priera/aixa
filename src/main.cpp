#include "platform/aixa_export.h"

#include <mainlib/audio/AudioWorkerFactory.h>
#include <mainlib/audio/note/NoteSetter.h>
#include <mainlib/gui/GraphicsEnvironmentFactory.h>
#include <mainlib/gui/MainEventFilter.h>

#include <QApplication>

static void setupSystemsForAudioStream(const std::filesystem::path& streamPath,
                                       GraphicsEnvironment& graphicsEnvironment,
                                       MainEventFilter& eventFilter,
                                       std::unique_ptr<AudioWorker>& audioWorker) {
    if (audioWorker) {
        audioWorker->stop();
        eventFilter.removeTransientCommands();
    }

    auto audioWorker_p = AudioWorkerFactory().buildWithInputStream(streamPath.string());
    audioWorker = std::unique_ptr<AudioWorker>(audioWorker_p);

    graphicsEnvironment.showAudioVisualizations();
    audioWorker->getSpectrogramGenerator().setReceiver(graphicsEnvironment.getSpectrogramConsumer());

    auto commandCollection = audioWorker->getCommandCollection();
    eventFilter.addCommandsFromCollection(commandCollection);

    audioWorker->start();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    const auto w = static_cast<int>(1920 * 3.0 / 4);
    const auto h = static_cast<int>(1080 * 3.0 / 4);
    const auto appSize = QSize(w, h);

    std::unique_ptr<AudioWorker> audioWorker = nullptr;
    auto graphicsEnvironment = GraphicsEnvironmentFactory::build(appSize);

    NoteSetter noteSetter;
    noteSetter.addObserver(graphicsEnvironment->getNotesListener());

    MainEventFilter mainEventFilter(noteSetter);
    app.installEventFilter(&mainEventFilter);

    auto audioSetupCallback = [&graphicsEnvironment, &audioWorker,
                               &mainEventFilter](const std::filesystem::path& streamPath) {
        setupSystemsForAudioStream(streamPath, *graphicsEnvironment, mainEventFilter, audioWorker);
    };
    QObject::connect(graphicsEnvironment.get(), &GraphicsEnvironment::streamReceived, audioSetupCallback);

    graphicsEnvironment->start();

    int ret = app.exec();

    if (audioWorker) {
        audioWorker->stop();
    }
    graphicsEnvironment->stop();

    return ret;
}
