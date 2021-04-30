#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_INSTRUCTIONSBANNER_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_INSTRUCTIONSBANNER_H

#include <mainlib/gui/gl/ShadersCollection.h>
#include <mainlib/gui/scene/ShadedRenderableObject.h>

#include "ImmutableTextBox.h"

class InstructionsBanner : public ShadedRenderableObject {
public:
    InstructionsBanner(TextureCollection& textureCollection) :
        ShadedRenderableObject(ShadersCollection::Vertex::FRONT_CHARACTER,
                               ShadersCollection::Fragment::CHARACTER,
                               Dimensions{0.9f, 1.125f, 0.1f}),
        textureCollection(&textureCollection), myTextBox() {}

protected:
    void init() override;

private:
    constexpr static int FONT_SIZE = 25;
    constexpr static float FONT_SCREEN_HEIGHT = 0.02f;
    constexpr static float RATIO = FONT_SCREEN_HEIGHT / static_cast<float>(FONT_SIZE);

    TextureCollection* textureCollection;

    std::shared_ptr<ImmutableTextBox> myTextBox;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_INSTRUCTIONSBANNER_H
