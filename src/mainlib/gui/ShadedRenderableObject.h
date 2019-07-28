#ifndef ALSAPLAYGROUND_SRC_MAINLIB_GUI_SHADEDRENDERABLEOBJECT_H
#define ALSAPLAYGROUND_SRC_MAINLIB_GUI_SHADEDRENDERABLEOBJECT_H

#include "mainlib/gui/RenderableObject.h"

#include <memory>

#include <QMatrix4x4>
#include <QString>

class ShadedRenderableObject : public RenderableObject {
public:
    ShadedRenderableObject(const QString &vertexShaderPath, const QString &fragmentShaderPath);

protected:
    void beforeRender(const QMatrix4x4 & projectionMatrix) override;
    void afterRender() override;

private:
    std::unique_ptr<QOpenGLShaderProgram> programContainer;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_GUI_SHADEDRENDERABLEOBJECT_H
