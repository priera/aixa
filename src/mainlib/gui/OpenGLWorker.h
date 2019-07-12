#ifndef ALSAPLAYGROUND_OPENGLWORKER_H
#define ALSAPLAYGROUND_OPENGLWORKER_H

#include <atomic>
#include <memory>
#include <vector>

#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QSurfaceFormat>

class QOpenGLShaderProgram;
class QOpenGLContext;

class OpenGLWorker : protected QOpenGLExtraFunctions {
public:

    OpenGLWorker(QOpenGLContext &context);
    virtual ~OpenGLWorker();

    void bindToSurface(QSurface *surface, int w, int h);

    void draw();

    void setSize(int w, int h);

private:

    QOpenGLContext *context;
    QSurface *surface;

    std::atomic<int> w;
    std::atomic<int> h;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    std::unique_ptr<QOpenGLShaderProgram> m_program;
    int m_frame;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int VBO, VAO, EBO;

};


#endif //ALSAPLAYGROUND_OPENGLWORKER_H
