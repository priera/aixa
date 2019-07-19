#ifndef ALSAPLAYGROUND_OPENGLWORKER_H
#define ALSAPLAYGROUND_OPENGLWORKER_H

#include <atomic>
#include <memory>
#include <vector>

#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QMatrix4x4>
#include <QtGui/QVector2D>

#include <ft2build.h>
#include FT_FREETYPE_H

class QImage;
class QOpenGLShaderProgram;
class QOpenGLContext;

class OpenGLWorker : protected QOpenGLExtraFunctions {
public:
    struct Character {
        GLuint textureID;   // ID handle of the glyph texture
        std::vector<unsigned int> size;    // Size of glyph
        std::vector<int> bearing;  // Offset from baseline to left/top of glyph
        long int advance;    // Horizontal offset to advance to next glyph
    };

    OpenGLWorker(QOpenGLContext &context);
    virtual ~OpenGLWorker();

    void bindToSurface(QSurface *surface, int w, int h);

    void draw();

    void setSize(int w, int h);

private:
    void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, QVector3D color);

    QOpenGLContext *context;
    QSurface *surface;

    std::atomic<int> w;
    std::atomic<int> h;
    std::atomic<float> aspectRatio;

    std::unique_ptr<QOpenGLShaderProgram> program;
    unsigned int m_frame;

    unsigned int VBO, VAO;

    QMatrix4x4 projection;

    std::map<GLchar, Character> characters;
    FT_Library ft;
};


#endif //ALSAPLAYGROUND_OPENGLWORKER_H
