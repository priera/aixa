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
        bool animateTextColor = false;
        Alignment alignment = Alignment::LEFT;
    };

    struct TextFormat {
        int pixelSize;
        float textRatio;
        float color = DEFAULT_COLOR;
    };

    ImmutableTextBox(QOpenGLShaderProgram &program,
                     std::string text,
                     BoxFormat boxFormat,
                     TextFormat textFormat,
                     TextureCollection &textureCollection);

    ~ImmutableTextBox() override = default;

protected:
    static constexpr float DEFAULT_COLOR = 0.7;

    void init() override;

    void doMyRender() override;

private:
    void computeXOffset();

    std::string text;
    BoxFormat boxFormat;
    TextFormat textFormat;
    TextureCollection *textureCollection;

    float xOffset;
    unsigned int vertexAttr, vertexBuff;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_IMMUTABLETEXTBOX_H
