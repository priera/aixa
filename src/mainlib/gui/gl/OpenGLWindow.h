#ifndef AIXA_OPENGLWINDOW_H
#define AIXA_OPENGLWINDOW_H

#include <memory>
#include <atomic>

#include <QWindow>
#include <QOpenGLFunctions>
#include "Scene.h"

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWindow(Scene &scene, std::unique_ptr<QOpenGLContext> &context);

    ~OpenGLWindow() override;

public slots:
    virtual void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;
    void resizeEvent(QResizeEvent *ev) override;

private:
    void init();

    bool initialized;

    Scene *scene;
    std::unique_ptr<QOpenGLContext> context;
};


#endif //AIXA_OPENGLWINDOW_H
