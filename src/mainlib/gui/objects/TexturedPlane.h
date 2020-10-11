#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H

#include <filesystem>

#include <mainlib/gui/scene/ShadedRenderableObject.h>

class TexturedPlane : public ShadedRenderableObject {
public:
    explicit TexturedPlane(std::filesystem::path texturePath);
    ~TexturedPlane() override = default;

protected:
    void init() override;
    void doMyRender() override;

private:
    std::filesystem::path texturePath;
};


#endif //AIXA_SRC_MAINLIB_GUI_OBJECTS_TEXTUREDPLANE_H
