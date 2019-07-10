
//#include "examples/AlsaExamples.h"

#include <thread>
#include <memory>

#include <QApplication>

#include "mainlib/gui/MainWindow.h"
#include "mainlib/gui/MainEventFilter.h"

#include "mainlib/gui/OpenGLTask.h"
#include "mainlib/gui/OpenGLWindow.h"

#include "mainlib/audio/AudioWorker.h"
#include "mainlib/audio/AudioBuilder.h"

#include "mainlib/audio/note/NotesProcessor.h"

#include "mainlib/buffers/CircularBuffer.h"

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

    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(3);
    format.setMinorVersion(3);

    OpenGLTask openGLTask(format);
    openGLTask.start();

    auto window = openGLTask.getWindow();
    window->show();

    /*
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

    auto audioThread = buildAudioThread(worker); */

    int ret = app.exec();

    openGLTask.stop();

    /*notesProcesor.stop();
    worker.stop();
    audioThread->join(); */

    return ret;
}

