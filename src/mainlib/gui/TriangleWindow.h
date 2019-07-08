#ifndef ALSAPLAYGROUND_TRIANGLEWINDOW_H
#define ALSAPLAYGROUND_TRIANGLEWINDOW_H

#include "mainlib/gui/OpenGLWindow.h"

class QOpenGLShaderProgram;

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize() override;
    void render() override;

private:
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};


#endif //ALSAPLAYGROUND_TRIANGLEWINDOW_H
