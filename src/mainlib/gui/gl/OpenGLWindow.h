#ifndef AIXA_OPENGLWINDOW_H
#define AIXA_OPENGLWINDOW_H

#include <mainlib/audio/note/Note.h>
#include <mainlib/gui/objects/CentralNoteManager.h>
#include <mainlib/gui/scene/Scene.h>

#include <QOpenGLFunctions>
#include <QWindow>
#include <atomic>
#include <memory>

class OpenGLWindow : public QWindow, public NotesListener, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLWindow(Scene &scene, std::unique_ptr<QOpenGLContext> &context, BitmapsProvider &bitmapsProvider,
                 TextureCollection &textureCollection);

    ~OpenGLWindow() override;

public slots:
    virtual void renderNow();

    void notifyNewValue(const Note &note) override;

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;
    void resizeEvent(QResizeEvent *ev) override;

private:
    void init();

    bool initialized;

    Scene *scene;
    std::unique_ptr<QOpenGLContext> context;
    std::unique_ptr<CentralNoteManager> centralNoteManager;
    BitmapsProvider *bitmapsProvider;
    TextureCollection *textureCollection;
};

#endif  // AIXA_OPENGLWINDOW_H
