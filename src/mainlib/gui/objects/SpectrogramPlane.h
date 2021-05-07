#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_SPECTROGRAMPLANE_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_SPECTROGRAMPLANE_H

#include <mainlib/gui/bitmap/BitmapBuilders.h>
#include <mainlib/gui/scene/ShadedRenderableObject.h>
#include <mainlib/gui/texture/DynamicTexture.h>

class SpectrogramPlane : public ShadedRenderableObject {
public:
    explicit SpectrogramPlane(SharedBitmapProvider bitmapProvider, const DynamicTexture& texture);

    ~SpectrogramPlane() noexcept override;

protected:
    void init() override;
    void doMyRender() override;

private:
    SharedBitmapProvider bitmapProvider;

    unsigned int VBO{0};
    unsigned int VAO{0};
    unsigned int EBO{0};
    DynamicTexture texture;

    std::vector<int> indices;
};

#endif  // AIXA_SRC_MAINLIB_GUI_OBJECTS_SPECTROGRAMPLANE_H
