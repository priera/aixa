#ifndef AIXA_SRC_MAINLIB_GUI_TEXTURE_DYNAMICTEXTURE_H
#define AIXA_SRC_MAINLIB_GUI_TEXTURE_DYNAMICTEXTURE_H

#include "Texture.h"

class DynamicTexture : public Texture {
public:
    DynamicTexture(unsigned int id, SharedBitmapProvider bitmapProvider);
    ~DynamicTexture() override = default;

protected:
    void update() override;

private:
    SharedBitmapProvider bitmapProvider;
};

#endif  // AIXA_SRC_MAINLIB_GUI_TEXTURE_DYNAMICTEXTURE_H
