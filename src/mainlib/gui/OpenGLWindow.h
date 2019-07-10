#ifndef ALSAPLAYGROUND_OPENGLWINDOW_H
#define ALSAPLAYGROUND_OPENGLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class QOpenGLShaderProgram;

class OpenGLWorker;

class OpenGLWindow : public QOpenGLWindow {
    Q_OBJECT

public:
    explicit OpenGLWindow(OpenGLWorker &worker, QOpenGLContext *context);

    //void setAnimating(bool moving) { m_moving = moving; }

signals:
    void readyGL();

protected:
    //bool event(QEvent *event) override;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    /*GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    bool m_moving; */

    OpenGLWorker *worker;
};


#endif //ALSAPLAYGROUND_OPENGLWINDOW_H
