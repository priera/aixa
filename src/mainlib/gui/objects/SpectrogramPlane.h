#ifndef AIXA_SRC_MAINLIB_GUI_OBJECTS_SPECTROGRAMPLANE_H
#define AIXA_SRC_MAINLIB_GUI_OBJECTS_SPECTROGRAMPLANE_H


#include <mainlib/gui/scene/ShadedRenderableObject.h>
#include <mainlib/gui/bitmap/BitmapsProvider.h>

class SpectrogramPlane : public ShadedRenderableObject {
public:
    explicit SpectrogramPlane(BitmapsProvider &bitmapsProvider);

    ~SpectrogramPlane() noexcept override;

protected:
    void init() override;
    void doMyRender() override;

private:
    BitmapsProvider *bitmapsProvider;

    unsigned int VBO{0};
    unsigned int VAO{0};
    unsigned int EBO{0};
    unsigned int texture{0};

    std::vector<int> indices;
};


#endif //AIXA_SRC_MAINLIB_GUI_OBJECTS_SPECTROGRAMPLANE_H
