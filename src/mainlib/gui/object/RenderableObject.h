#ifndef ALSAPLAYGROUND_RENDERABLEOBJECT_H
#define ALSAPLAYGROUND_RENDERABLEOBJECT_H

#include <map>
#include <mutex>

#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QOpenGLShaderProgram>

#include <QtGui/QMatrix4x4>

class RenderableObject : protected QOpenGLExtraFunctions {
public:
    RenderableObject(QOpenGLShaderProgram &program);
    virtual ~RenderableObject();

    void update();
    void render(QMatrix4x4 & projectionMatrix);

    void moveCenterAt(float x, float y, float z);
    void rotate(float degrees);

    void addChildObject(float z, RenderableObject *object);

protected:
    RenderableObject();

    void setProgram(QOpenGLShaderProgram &program) {
        this->program = &program;
    }

    virtual void doMyUpdate();
    virtual void updateDone();

    virtual void doMyRender();
    virtual void beforeRender(const QMatrix4x4 & projectionMatrix);
    virtual void afterRender();

    virtual void applyChildTransformations(RenderableObject *pObject);

    float w, h, d;

    std::map<int, RenderableObject *> children;

    QOpenGLShaderProgram *program;

private:
    QMatrix4x4 updateMatrix;
    QMatrix4x4 renderMatrix;

    std::mutex updateMutex;
};

#endif //ALSAPLAYGROUND_RENDERABLEOBJECT_H
