#ifndef ALSAPLAYGROUND_OPENGLWORKER_H
#define ALSAPLAYGROUND_OPENGLWORKER_H

#include <atomic>
#include <memory>
#include <vector>

#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QMatrix4x4>

class QImage;
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
    std::atomic<float> aspectRatio;

    std::unique_ptr<QOpenGLShaderProgram> program;
    int m_frame;

    std::vector<float> vertices;
    std::vector<int> indices;

    unsigned int VBO, VAO, EBO;
    std::vector<unsigned int> textures;

    std::unique_ptr<QImage> textureImage;
    std::unique_ptr<QImage> happyImage;

    QMatrix4x4 model, view, projection;

};


#endif //ALSAPLAYGROUND_OPENGLWORKER_H
