#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H

#include <mainlib/gui/bitmap/BitmapBuilders.h>
#include <mainlib/gui/scene/ShadedRenderableObject.h>

#include <filesystem>

class TexturedPlane : public ShadedRenderableObject {
public:
    TexturedPlane(BitmapBuilders &bitmapsProvider, std::filesystem::path texturePath);
    ~TexturedPlane() noexcept override;

protected:
    void init() override;
    void doMyRender() override;

private:
    BitmapBuilders *bitmapsProvider;
    std::filesystem::path texturePath;

    unsigned int VBO{0};
    unsigned int VAO{0};
    unsigned int EBO{0};
    unsigned int texture{0};

    std::vector<int> indices;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H
