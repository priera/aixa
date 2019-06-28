
//#include "AlsaExamples.h"

#include <thread>
#include <memory>

#include <QApplication>

#include "MainWindow.h"
#include "MainEventFilter.h"

#include "AudioWorker.h"
#include "AudioBuilder.h"

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
    auto environment = std::unique_ptr<AlsaEnvironment>(environment_p);
    AudioWorker worker(basicParameters, environment);

    MainEventFilter mainEventFilter;
    app.installEventFilter(&mainEventFilter);

    MainWindow mainWindow;
    mainWindow.show();

    auto audioThread = buildAudioThread(worker);

    int ret = app.exec();

    worker.stop();
    audioThread->join();

    return ret;
}

