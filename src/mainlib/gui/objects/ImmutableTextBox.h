#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H

#include <mainlib/gui/scene/RenderableObject.h>
#include <mainlib/gui/texture/TextureCollection.h>

class ImmutableTextBox : public RenderableObject {
public:
    ImmutableTextBox(QOpenGLShaderProgram &program, std::string text, unsigned int pixelSize, float x,
                     float y, float ratio, TextureCollection &textureCollection);
    ~ImmutableTextBox() override = default;

protected:
    void init() override;

    void doMyRender() override;

private:
    std::string text;
    unsigned int pixelSize;
    float x, y, ratio;
    TextureCollection *textureCollection;

    unsigned int vertexAttr, vertexBuff;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H
