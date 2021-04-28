#ifndef AIXA_SRC_MAINLIB_GUI_GL_DRAWWIDGET_H
#define AIXA_SRC_MAINLIB_GUI_GL_DRAWWIDGET_H

#include <mainlib/gui/objects/CentralNoteManager.h>
#include <mainlib/gui/scene/Scene.h>

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPaintEvent>
#include <QResizeEvent>
#include <memory>

class DrawWidget : public QOpenGLWidget, public NotesListener, protected QOpenGLFunctions {
    Q_OBJECT
public:
    DrawWidget(Scene &scene, BitmapBuilders &bitmapBuilders);

signals:
    void initialized(QOpenGLContext *context);

public slots:
    void notifyNewValue(const Note &note) override;

    void renderNow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    //    bool event(QEvent* event) override;
    //    void paintEvent(QPaintEvent *e) override;

private:
    Scene *scene;
    BitmapBuilders *bitmapBuilders;

    std::unique_ptr<CentralNoteManager> centralNoteManager;
    TextureCollection *textureCollection;
};

#endif  // AIXA_SRC_MAINLIB_GUI_GL_DRAWWIDGET_H
