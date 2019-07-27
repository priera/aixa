
//#include "examples/AlsaExamples.h"

#include <thread>
#include <memory>
#include <chrono>

#include <QApplication>

#include "mainlib/gui/MainEventFilter.h"

#include "mainlib/gui/OpenGLTask.h"
#include "mainlib/gui/OpenGLWindow.h"
#include "mainlib/gui/CentralNoteManager.h"

#include "mainlib/gui/gl/GLContextManager.h"

#include "mainlib/audio/AudioWorker.h"
#include "mainlib/audio/AudioBuilder.h"

#include "mainlib/audio/note/NoteSetter.h"

using namespace std::chrono_literals;

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

    auto winContext_p = GLContextManager::getInstance().createContext();
    std::unique_ptr<QOpenGLContext> winContext(winContext_p);

    OpenGLWindow win(winContext);

    float w = 1920 * 3.0 / 4;
    float h = 1080 * 3.0 / 4;
    win.resize(w, h);
    win.show();

    OpenGLTask openGLTask(win);
    openGLTask.start();

/*    QSurfaceFormat format;
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
    noteSetter.addObserver(&worker);

    MainEventFilter mainEventFilter(commandCollection, noteSetter);
    app.installEventFilter(&mainEventFilter);

    auto audioThread = buildAudioThread(worker);

    //Show the window just right before the application starts
    auto window = openGLTask.getWindow();
    window->show();

 */
    //std::this_thread::sleep_for(50ms);

    //noteSetter.addObserver(openGLTask.getCentralNoteManager());

    int ret = app.exec();

   /* openGLTask.stop();

    worker.stop();
    audioThread->join();
*/

    GLContextManager::getInstance().release();

    return ret;
}

