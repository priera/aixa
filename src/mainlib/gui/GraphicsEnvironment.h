#ifndef AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H
#define AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H

#include <aixa_export.h>
#include <mainlib/audio/note/Note.h>
#include <mainlib/gui/gl/DrawWidget.h>
#include <mainlib/gui/gl/DrawingWorker.h>
#include <mainlib/gui/scene/Scene.h>
#include <mainlib/gui/scene/SceneState.h>

#include <memory>

class LIB_EXPORT GraphicsEnvironment : public QObject {
    Q_OBJECT
public:
    GraphicsEnvironment(std::unique_ptr<Scene> scene,
                        std::unique_ptr<DrawingWorker> drawingWorker,
                        std::unique_ptr<DrawWidget> mainWin,
                        std::unique_ptr<BitmapBuilders> bitmapsProvider);

    ~GraphicsEnvironment() override = default;

    void start() { mainWindow->show(); }

    void stop() { drawingWorker->stop(); }

    void showAudioVisualizations();

    std::shared_ptr<NotesListener> getNotesListener() { return mainWindow; }

    std::shared_ptr<aixa::math::SpectrogramConsumer> getSpectrogramConsumer() {
        return bitmapsProvider->getSpectrogramConsumer();
    }

signals:
    LIB_EXPORT void streamReceived(const std::filesystem::path& path);

private slots:
    void setupUsingContext(QOpenGLContext* guiContext);
    void checkProposedStream(const QUrl& url);

private:
    std::unique_ptr<Scene> scene;
    std::unique_ptr<DrawingWorker> drawingWorker;
    std::shared_ptr<DrawWidget> mainWindow;
    std::unique_ptr<BitmapBuilders> bitmapsProvider;

    std::unique_ptr<TextureCollection> textureCollection;
    std::unique_ptr<SceneState> sceneState;
};

#endif  // AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H
