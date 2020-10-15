#ifndef AIXA_SRC_MAINLIB_GUI_SHADEDRENDERABLEOBJECT_H
#define AIXA_SRC_MAINLIB_GUI_SHADEDRENDERABLEOBJECT_H

#include "RenderableObject.h"

#include <memory>

#include <QMatrix4x4>
#include <QString>

class ShadedRenderableObject : public RenderableObject {
public:
    ShadedRenderableObject(const QString &vertexShaderPath, const QString &fragmentShaderPath, Dimensions dimensions);

protected:
    void beforeRender(const QMatrix4x4 & projectionMatrix) override;
    void afterRender() override;

private:
    std::unique_ptr<QOpenGLShaderProgram> programContainer;
};


#endif //AIXA_SRC_MAINLIB_GUI_SHADEDRENDERABLEOBJECT_H
