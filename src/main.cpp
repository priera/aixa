
//#include "examples/AlsaExamples.h"

#include <thread>
#include <memory>

#include <QApplication>

#include "mainlib/gui/MainWindow.h"
#include "mainlib/gui/MainEventFilter.h"

#include "mainlib/audio/AudioWorker.h"
#include "mainlib/audio/AudioBuilder.h"
#include "mainlib/audio/NoteSetter.h"

//for tests these ones
/*#include "audio/SineGenerator.h"
#include "audio/Buffers.h"
#include "audio/utils.h"
*/

std::thread * buildAudioThread(AudioWorker & worker)
{
    auto f = [&worker](){
        worker.start();
    };

    return new std::thread(f);
}

/*static void runTests() {
    Buffers b(1, 4410, SND_PCM_FORMAT_S16);
    SineGenerator g(b, 4410, 44100);

    g.fillFrame(622.25, 100);

    QJsonArray samples;
    audioUtils::dumpSignal(b, samples);
    audioUtils::writeSamplesTo(samples, "G.json");
} */

int main(int argc, char *argv[]) {
  /*  AlsaExamples examples(argc, argv);
    examples.run(); */

    /*runTests();

    return 0; */

    QApplication app(argc, argv);

    AudioBuilder audioBuilder;
    auto basicParameters = getDefaultAudioParameters();
    auto environment_p = audioBuilder.setupAudioEnvironment(basicParameters);
    auto environment = std::unique_ptr<AudioEnvironment>(environment_p);

    AudioWorker worker(environment);
    auto commandCollection = worker.buildCommandCollection();
    auto noteSetter = worker.getNoteSetter();

    MainEventFilter mainEventFilter(commandCollection, *noteSetter);
    app.installEventFilter(&mainEventFilter);

    MainWindow mainWindow;
    mainWindow.show();

    auto audioThread = buildAudioThread(worker);

    int ret = app.exec();

    worker.stop();
    audioThread->join();

    return ret;
}

