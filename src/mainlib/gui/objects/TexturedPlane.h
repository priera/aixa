#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H

#include <filesystem>

#include <mainlib/gui/scene/ShadedRenderableObject.h>

class TexturedPlane : public ShadedRenderableObject {
public:
    explicit TexturedPlane(std::filesystem::path texturePath);
    ~TexturedPlane() noexcept override;

protected:
    void init() override;
    void doMyRender() override;

private:
    std::filesystem::path texturePath;

    unsigned int VBO{0};
    unsigned int VAO{0};
    unsigned int EBO{0};
    unsigned int texture{};
};


#endif //AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H
