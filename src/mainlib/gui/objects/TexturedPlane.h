#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H

#include <filesystem>

#include <mainlib/gui/scene/ShadedRenderableObject.h>
#include <mainlib/gui/bitmap/BitmapsProvider.h>

class TexturedPlane : public ShadedRenderableObject {
public:
    TexturedPlane(BitmapsProvider &bitmapsProvider, std::filesystem::path texturePath);
    ~TexturedPlane() noexcept override;

protected:
    void init() override;
    void doMyRender() override;

private:
    BitmapsProvider *bitmapsProvider;
    std::filesystem::path texturePath;

    unsigned int VBO{0};
    unsigned int VAO{0};
    unsigned int EBO{0};
    unsigned int texture{0};

    std::vector<int> indices;
};


#endif //AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H
