#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H

#include <mainlib/gui/scene/RenderableObject.h>
#include <mainlib/gui/texture/TextureCollection.h>

class ImmutableTextBox : public RenderableObject {
public:
    enum class Alignment
    {
        LEFT,
        CENTER
    };

    struct BoxFormat {
        float left;
        float top;
        Alignment alignment = Alignment::LEFT;
    };

    ImmutableTextBox(QOpenGLShaderProgram &program,
                     std::string text,
                     BoxFormat boxFormat,
                     unsigned int pixelSize,
                     float ratio,
                     TextureCollection &textureCollection);
    ~ImmutableTextBox() override = default;

protected:
    void init() override;

    void doMyRender() override;

private:
    void computeXOffset();

    std::string text;
    BoxFormat boxFormat;
    unsigned int pixelSize;
    float ratio;
    TextureCollection *textureCollection;

    float xOffset;
    unsigned int vertexAttr, vertexBuff;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H
