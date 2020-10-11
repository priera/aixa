#ifndef AIXA_OPENGLWINDOW_H
#define AIXA_OPENGLWINDOW_H

#include <memory>
#include <atomic>

#include <QWindow>
#include <QOpenGLFunctions>

#include <mainlib/audio/note/Note.h>
#include <mainlib/gui/CentralNoteManager.h>

#include "mainlib/gui/scene/Scene.h"

class OpenGLWindow :
        public QWindow,
        public NotesListener,
        protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWindow(Scene &scene, std::unique_ptr<QOpenGLContext> &context);

    ~OpenGLWindow() override;

public slots:
    virtual void renderNow();

    void notifyNewValue(const Note& note) override;

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
};


#endif //AIXA_OPENGLWINDOW_H
