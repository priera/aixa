
//#include "examples/AlsaExamples.h"

#include <QApplication>

#include <mainlib/audio/AudioWorkerFactory.h>
#include <mainlib/math/dft/MatrixFourierTransform.h>
#include <mainlib/math/SineGenerator.h>
#include <mainlib/math/dft/CooleyTukeyFFT.h>
#include <mainlib/math/dft/FourierTransformFactory.h>

#include "mainlib/gui/MainEventFilter.h"

#include "mainlib/gui/gl/OpenGLTask.h"
#include "mainlib/gui/gl/OpenGLWindow.h"
#include "mainlib/gui/gl/GLContextManager.h"

#include "mainlib/gui/CentralNoteManager.h"
#include "mainlib/audio/note/NoteSetter.h"

using namespace std::chrono_literals;

using namespace aixa::math;

//static const auto STREAM = "/home/pedro/alsaTests/amics.wav";
static const auto STREAM = "??";

int main(int argc, char *argv[]) {
    using namespace std::chrono_literals;

    /*auto fft = getFourierTransformFactory(FourierTransformFactory::Implementations::FFT).build(16);
    return 0; */

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

    auto audioWorker = AudioWorkerFactory().buildWithInputStream(STREAM);
    audioWorker->start();

    auto commandCollection = audioWorker->getCommandCollection();

    MainEventFilter mainEventFilter(commandCollection, noteSetter);
    app.installEventFilter(&mainEventFilter);

    openGLTask.start();

    int ret = app.exec();

    audioWorker->stop();
    openGLTask.quit();

    GLContextManager::getInstance().release();

    return ret;
}

