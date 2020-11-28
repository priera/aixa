#ifndef AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H
#define AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H

#include <mainlib/audio/note/Note.h>
#include <mainlib/gui/gl/DrawingWorker.h>
#include <mainlib/gui/gl/GLContextManager.h>
#include <mainlib/gui/gl/OpenGLWindow.h>
#include <mainlib/gui/scene/Scene.h>

#include <memory>

class GraphicsEnvironment {
public:
    GraphicsEnvironment(std::unique_ptr<Scene> scene, std::unique_ptr<DrawingWorker> drawingWorker,
                        std::unique_ptr<OpenGLWindow> openGLWindow,
                        std::unique_ptr<BitmapsProvider> bitmapsProvider,
                        std::unique_ptr<TextureCollection> textureCollection) :
        scene(std::move(scene)),
        drawingWorker(std::move(drawingWorker)),
        openGLWindow(std::move(openGLWindow)),
        bitmapsProvider(std::move(bitmapsProvider)),
        textureCollection(std::move(textureCollection)) {}

    virtual ~GraphicsEnvironment() = default;

    void start() {
        drawingWorker->start();
        openGLWindow->show();
    }

    void stop() {
        drawingWorker->stop();
        GLContextManager::release();
    }

    std::shared_ptr<NotesListener> getNotesListener() { return openGLWindow; }

    std::shared_ptr<aixa::math::SpectrogramConsumer> getSpectrogramConsumer() {
        return bitmapsProvider->getSpectrogramConsumer();
    }

private:
    std::unique_ptr<Scene> scene;
    std::unique_ptr<DrawingWorker> drawingWorker;
    std::shared_ptr<OpenGLWindow> openGLWindow;
    std::unique_ptr<BitmapsProvider> bitmapsProvider;
    std::unique_ptr<TextureCollection> textureCollection;
};

#endif  // AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H
