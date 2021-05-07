#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_YSCALE_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_YSCALE_H

#include <mainlib/gui/gl/ShadersCollection.h>
#include <mainlib/gui/scene/ShadedRenderableObject.h>
#include <mainlib/gui/texture/TextureCollection.h>

class YScale : public ShadedRenderableObject {
public:
    static YScale *buildLinear(float scaleMax, unsigned int steps, TextureCollection &textureCollection);
    static YScale *buildLogarithmic(float scaleMax, TextureCollection &textureCollection);

    ~YScale() override = default;

protected:
    void init() override;

private:
    using ScaleContent = std::vector<std::pair<float, std::string>>;

    constexpr static int FONT_SIZE = 20;
    constexpr static float FONT_SCREEN_HEIGHT = 0.02f;
    constexpr static float RATIO = FONT_SCREEN_HEIGHT / static_cast<float>(FONT_SIZE);

    YScale(ScaleContent textBoxesContent, TextureCollection &textureCollection) :
        ShadedRenderableObject(ShadersCollection::Vertex::FRONT_CHARACTER,
                               ShadersCollection::Fragment::CHARACTER, Dimensions{0.9f, 1.125f, 0.1f}),
        scaleContent(std::move(textBoxesContent)),
        textureCollection(&textureCollection) {}

    ScaleContent scaleContent;
    TextureCollection *textureCollection;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_YSCALE_H
