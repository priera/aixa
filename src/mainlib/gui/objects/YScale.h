#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_YSCALE_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_YSCALE_H

#include <mainlib/gui/gl/ShadersCollection.h>
#include <mainlib/gui/scene/ShadedRenderableObject.h>
#include <mainlib/gui/texture/TextureCollection.h>

class YScale : public ShadedRenderableObject {
public:
    explicit YScale(float scaleMax, TextureCollection &textureCollection) :
        ShadedRenderableObject(ShadersCollection::Vertex::FRONT_CHARACTER,
                               ShadersCollection::Fragment::CHARACTER, Dimensions{0.9f, 1.125f, 0.1f}),
        scaleMax(scaleMax),
        textureCollection(&textureCollection) {}

    ~YScale() override = default;

protected:
    void init() override;

private:
    constexpr static unsigned int STEPS = 10;
    constexpr static int FONT_SIZE = 20;
    constexpr static float FONT_SCREEN_HEIGHT = 0.02f;
    constexpr static float RATIO = FONT_SCREEN_HEIGHT / static_cast<float>(FONT_SIZE);

    float scaleMax;
    TextureCollection *textureCollection;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_YSCALE_H
