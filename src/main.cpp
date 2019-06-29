
//#include "examples/AlsaExamples.h"

#include <thread>
#include <memory>

#include <QApplication>

#include "gui/MainWindow.h"
#include "gui/MainEventFilter.h"

#include "audio/AudioWorker.h"
#include "audio/AudioBuilder.h"
#include "audio/NoteSetter.h"

std::thread * buildAudioThread(AudioWorker & worker)
{
    auto f = [&worker](){
        worker.start();
    };

    return new std::thread(f);
}

int main(int argc, char *argv[]) {
  /*  AlsaExamples examples(argc, argv);
    examples.run(); */

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

