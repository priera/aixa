#ifndef ALSAPLAYGROUND_OPENGLWINDOW2_H
#define ALSAPLAYGROUND_OPENGLWINDOW2_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class QOpenGLShaderProgram;

class OpenGLWindow2 : public QOpenGLWindow, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit OpenGLWindow2();

    void setAnimating(bool moving) { m_moving = moving; }

protected:
    bool event(QEvent *event) override;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    bool m_moving;
};


#endif //ALSAPLAYGROUND_OPENGLWINDOW2_H
