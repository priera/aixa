#include <mainlib/audio/AudioWorkerFactory.h>
#include <mainlib/audio/note/NoteSetter.h>
#include <mainlib/gui/GraphicsEnvironmentFactory.h>
#include <mainlib/gui/MainEventFilter.h>

#include <QApplication>

// static const auto STREAM = "??";

static void setupAudioWithStream(const std::filesystem::path& streamPath,
                                 GraphicsEnvironment& graphicsEnvironment,
                                 std::unique_ptr<AudioWorker>& audioWorker) {
    if (audioWorker) {
        audioWorker->stop();
    }

    auto audioWorker_p = AudioWorkerFactory().buildWithInputStream(streamPath);
    audioWorker = std::unique_ptr<AudioWorker>(audioWorker_p);

    audioWorker->getSpectrogramGenerator().setReceiver(graphicsEnvironment.getSpectrogramConsumer());

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

    auto audioSetupFunc = [&graphicsEnvironment, &audioWorker](const std::filesystem::path& streamPath) {
        setupAudioWithStream(streamPath, *graphicsEnvironment, audioWorker);
    };
    QObject::connect(graphicsEnvironment.get(), &GraphicsEnvironment::streamReceived, audioSetupFunc);

    //    auto audioWorker_p = AudioWorkerFactory().buildWithInputStream(STREAM);
    //    auto audioWorker2 = std::unique_ptr<AudioWorker>(audioWorker_p);
    //
    //    audioWorker2->getSpectrogramGenerator().setReceiver(graphicsEnvironment->getSpectrogramConsumer());
    //
    //    auto commandCollection = audioWorker2->getCommandCollection();
    //
    //    MainEventFilter mainEventFilter(commandCollection, noteSetter);
    //    app.installEventFilter(&mainEventFilter);

    graphicsEnvironment->start();
    // audioWorker2->start();

    int ret = app.exec();

    if (audioWorker) {
        audioWorker->stop();
    }
    // audioWorker2->stop();
    graphicsEnvironment->stop();

    return ret;
}
