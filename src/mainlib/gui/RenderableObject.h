#ifndef ALSAPLAYGROUND_RENDERABLEOBJECT_H
#define ALSAPLAYGROUND_RENDERABLEOBJECT_H

#include <map>

#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QOpenGLShaderProgram>

#include <QtGui/QMatrix4x4>

class RenderableObject : protected QOpenGLExtraFunctions {
public:
    RenderableObject(const QMatrix4x4 &projectionMatrix, QOpenGLShaderProgram &program);
    virtual ~RenderableObject();

    void render();

    void moveCenterAt(float x, float y, float z);
    void rotate(float degrees);

    void addChildObject(float z, RenderableObject *object);

protected:
    RenderableObject(const QMatrix4x4 &projectionMatrix);

    void setProgram(QOpenGLShaderProgram &program) {
        this->program = &program;
    }

    virtual void doMyRender();
    virtual void beforeRender();
    virtual void afterRender();

    float w, h, d;

    std::map<int, RenderableObject *> children;

    QOpenGLShaderProgram *program;

private:
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 modelMatrix;

};

#endif //ALSAPLAYGROUND_RENDERABLEOBJECT_H
