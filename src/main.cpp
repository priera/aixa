
//#include "examples/AlsaExamples.h"

#include <thread>
#include <memory>

#include <QApplication>

#include "mainlib/gui/MainEventFilter.h"

#include "mainlib/gui/OpenGLTask.h"
#include "mainlib/gui/OpenGLWindow.h"

#include "mainlib/audio/AudioWorker.h"
#include "mainlib/audio/AudioBuilder.h"

#include "mainlib/audio/note/NoteSetter.h"

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
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);

    OpenGLTask openGLTask(format);
    openGLTask.start();

    AudioBuilder audioBuilder;
    auto basicParameters = getDefaultAudioParameters();
    auto environment_p = audioBuilder.setupAudioEnvironment(basicParameters);
    auto environment = std::unique_ptr<AudioEnvironment>(environment_p);

    AudioWorker worker(environment);
    auto commandCollection = worker.buildCommandCollection();

    NoteSetter noteSetter;
    noteSetter.addObserver(worker);

    MainEventFilter mainEventFilter(commandCollection, noteSetter);
    app.installEventFilter(&mainEventFilter);

    auto audioThread = buildAudioThread(worker);

    //Show the window just right before the application starts
    auto window = openGLTask.getWindow();
    window->show();

    int ret = app.exec();

    openGLTask.stop();

    worker.stop();
    audioThread->join();

    return ret;
}

