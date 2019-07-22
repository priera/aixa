#ifndef ALSAPLAYGROUND_OPENGLWORKER_H
#define ALSAPLAYGROUND_OPENGLWORKER_H

#include <atomic>
#include <memory>
#include <vector>

#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QMatrix4x4>

class QOpenGLShaderProgram;
class QOpenGLContext;

class RenderableObject;
class CentralNoteManager;

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

    void setNoteManager(CentralNoteManager &noteManager) {
        this->centralNoteManager = &noteManager;
    }

    void draw();

    void setSize(int w, int h);

    QMatrix4x4 getProjectionMatrix() { return projection; }

private:
    QOpenGLContext *context;
    QSurface *surface;

    std::atomic<int> w;
    std::atomic<int> h;

    unsigned int m_frame;

    QMatrix4x4 projection;

    CentralNoteManager *centralNoteManager;
};


#endif //ALSAPLAYGROUND_OPENGLWORKER_H
