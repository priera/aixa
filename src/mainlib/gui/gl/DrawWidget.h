#ifndef AIXA_SRC_MAINLIB_GUI_GL_DRAWWIDGET_H
#define AIXA_SRC_MAINLIB_GUI_GL_DRAWWIDGET_H

#include <mainlib/gui/objects/CentralNoteManager.h>
#include <mainlib/gui/scene/Scene.h>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <memory>

class DrawWidget : public QOpenGLWidget, public NotesListener, protected QOpenGLFunctions {
    Q_OBJECT
public:
    DrawWidget(Scene &scene);

signals:
    void initialized(QOpenGLContext *context);
    void urlDropped(const QUrl &url);

public slots:
    void notifyNewValue(const Note &note) override;

    void renderNow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;

private:
    Scene *scene;

    std::unique_ptr<CentralNoteManager> centralNoteManager;
    TextureCollection *textureCollection;
};

#endif  // AIXA_SRC_MAINLIB_GUI_GL_DRAWWIDGET_H
