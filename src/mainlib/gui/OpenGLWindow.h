#ifndef ALSAPLAYGROUND_OPENGLWINDOW_H
#define ALSAPLAYGROUND_OPENGLWINDOW_H

#include <memory>

#include <QWindow>
#include <QOpenGLFunctions>

class Scene;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWindow(std::unique_ptr<QOpenGLContext> &context);

    virtual ~OpenGLWindow();

    void setScene(Scene *scene) { this->scene = scene; }

public slots:
    virtual void render();
    virtual void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;
    void resizeEvent(QResizeEvent *ev) override;

private:
    void init();

    bool initialized;
    std::unique_ptr<QOpenGLContext> context;

    Scene *scene;
};


#endif //ALSAPLAYGROUND_OPENGLWINDOW_H
