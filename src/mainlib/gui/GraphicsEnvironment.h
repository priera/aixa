#ifndef AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H
#define AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H

#include <mainlib/audio/note/Note.h>
#include <mainlib/gui/gl/DrawWidget.h>
#include <mainlib/gui/gl/DrawingWorker.h>
#include <mainlib/gui/scene/Scene.h>

#include <memory>

class GraphicsEnvironment : public QObject {
    Q_OBJECT
public:
    GraphicsEnvironment(std::unique_ptr<Scene> scene,
                        std::unique_ptr<DrawingWorker> drawingWorker,
                        std::unique_ptr<DrawWidget> mainWin,
                        std::unique_ptr<BitmapBuilders> bitmapsProvider);

    ~GraphicsEnvironment() override = default;

    void start() { mainWindow->show(); }

    void stop() { drawingWorker->stop(); }

    std::shared_ptr<NotesListener> getNotesListener() { return mainWindow; }

    std::shared_ptr<aixa::math::SpectrogramConsumer> getSpectrogramConsumer() {
        return bitmapsProvider->getSpectrogramConsumer();
    }

signals:
    void streamReceived(const std::filesystem::path& path);

private slots:
    void startWorker(QOpenGLContext* guiContext);
    void checkProposedStream(const QUrl& url);

private:
    std::unique_ptr<Scene> scene;
    std::unique_ptr<DrawingWorker> drawingWorker;
    std::shared_ptr<DrawWidget> mainWindow;
    std::unique_ptr<BitmapBuilders> bitmapsProvider;
};

#endif  // AIXA_SRC_MAINLIB_GUI_GRAPHICSENVIRONMENT_H
