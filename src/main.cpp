
//#include "examples/AlsaExamples.h"

#include <thread>
#include <memory>
#include <chrono>

#include <QApplication>
#include <mainlib/stream/wav/WavReader.h>

#include "mainlib/gui/MainEventFilter.h"

#include "mainlib/gui/gl/OpenGLTask.h"
#include "mainlib/gui/gl/OpenGLWindow.h"
#include "mainlib/gui/CentralNoteManager.h"

#include "mainlib/gui/gl/GLContextManager.h"

#include "mainlib/audio/AudioWorkerOld.h"
#include "mainlib/audio/AudioBuilder.h"

#include "mainlib/audio/note/NoteSetter.h"

using namespace std::chrono_literals;

std::thread * buildAudioThread(AudioWorkerOld & worker)
{
    auto f = [&worker](){
        worker.start();
    };

    return new std::thread(f);
}

int main(int argc, char *argv[]) {
  /*  AlsaExamples examples(argc, argv);
    examples.run(); */

    /*WavReader wavReader("/home/pedro/alsaTests/amics.wav");
    wavReader.run(); */

    QApplication app(argc, argv);

    OpenGLWindow win;
    float w = 1920 * 3.0 / 4;
    float h = 1080 * 3.0 / 4;
    win.resize(w, h);
    win.show();

    OpenGLTask openGLTask(win);

    NoteSetter noteSetter;

    QObject::connect(&openGLTask, &OpenGLTask::sceneBuilt, [&win, &openGLTask, &noteSetter]() {
        win.setScene(openGLTask.getScene());
        win.setReady();
        noteSetter.addObserver(openGLTask.getCentralNoteManager());
    });

    /*AudioBuilder audioBuilder;
    auto basicParameters = getDefaultAudioParameters();
    auto environment_p = audioBuilder.setupAudioEnvironment(basicParameters);
    auto environment = std::unique_ptr<AudioEnvironment>(environment_p);

    AudioWorkerOld worker(environment);
    auto commandCollection = worker.buildCommandCollection();

    noteSetter.addObserver(&worker);

    MainEventFilter mainEventFilter(commandCollection, noteSetter);
    app.installEventFilter(&mainEventFilter);

    auto audioThread = buildAudioThread(worker); */

    openGLTask.start();

    int ret = app.exec();

    //worker.stop();

    openGLTask.quit();
    //audioThread->join();

    GLContextManager::getInstance().release();

    return ret;
}

