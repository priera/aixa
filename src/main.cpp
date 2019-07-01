
//#include "examples/AlsaExamples.h"

#include <thread>
#include <memory>

#include <QApplication>

#include "mainlib/gui/MainWindow.h"
#include "mainlib/gui/MainEventFilter.h"

#include "mainlib/audio/AudioWorker.h"
#include "mainlib/audio/AudioBuilder.h"

#include "mainlib/audio/note/NotesProcessor.h"

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

    NotesProcessor notesProcesor(worker);
    notesProcesor.start();

    MainEventFilter mainEventFilter(commandCollection, *notesProcesor.getNoteSetter());
    app.installEventFilter(&mainEventFilter);

    MainWindow mainWindow;
    mainWindow.show();

    auto audioThread = buildAudioThread(worker);

    int ret = app.exec();

    notesProcesor.stop();
    worker.stop();
    audioThread->join();

    return ret;
}

