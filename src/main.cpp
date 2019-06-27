
//#include "AlsaExamples.h"

#include <QApplication>
#include <thread>

#include "MainWindow.h"
#include "MainEventFilter.h"

#include "AudioWorker.h"

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

    AudioWorker worker;

    MainEventFilter mainEventFilter;
    app.installEventFilter(&mainEventFilter);

    MainWindow mainWindow;
    mainWindow.show();

    auto audioThread = buildAudioThread(worker);

    int ret = app.exec();

    audioThread->join();

    return ret;
}

