#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_INSTRUCTIONSBANNER_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_INSTRUCTIONSBANNER_H

#include <mainlib/gui/gl/ShadersCollection.h>
#include <mainlib/gui/scene/ShadedRenderableObject.h>

#include <QTimer>

#include "ImmutableTextBox.h"

class InstructionsBanner : public ShadedRenderableObject {
public:
    explicit InstructionsBanner(TextureCollection& textureCollection);
    ~InstructionsBanner() override;

protected:
    void init() override;

private slots:
    void changeInstruction();

private:
    constexpr static int FONT_SIZE = 25;
    constexpr static float FONT_SCREEN_HEIGHT = 0.035f;
    constexpr static float RATIO = FONT_SCREEN_HEIGHT / static_cast<float>(FONT_SIZE);

    TextureCollection* textureCollection;

    std::unique_ptr<QTimer> timer;
    std::vector<std::string> instructionsSet;
    std::size_t currentInstruction;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_INSTRUCTIONSBANNER_H
