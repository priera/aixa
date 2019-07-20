#ifndef ALSAPLAYGROUND_RENDERABLEOBJECT_H
#define ALSAPLAYGROUND_RENDERABLEOBJECT_H

#include <memory>

#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QOpenGLShaderProgram>

#include <QtGui/QMatrix4x4>

class RenderableObject : protected QOpenGLExtraFunctions {
public:
    RenderableObject(const QMatrix4x4 &projectionMatrix, const QString &vertexShaderPath, const QString &fragmentShaderPath);
    virtual ~RenderableObject();

    void render();

    void moveCenterAt(float x, float y);
    void rotate(float degrees);

protected:
    virtual void doMyRender()= 0;

    std::unique_ptr<QOpenGLShaderProgram> program;
    float w, h;

private:
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 modelMatrix;
};

#endif //ALSAPLAYGROUND_RENDERABLEOBJECT_H
