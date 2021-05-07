#include "DynamicTexture.h"

#include <mainlib/gui/gl/utils.h>

DynamicTexture::DynamicTexture(unsigned int id, SharedBitmapProvider bitmapProvider) :
    Texture(id, bitmapProvider->get()), bitmapProvider(bitmapProvider) {}

void DynamicTexture::update() {
    this->bitmap = bitmapProvider->get();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bitmap.columns, bitmap.rows, bitmap.glStorage, GL_UNSIGNED_BYTE,
                    &bitmap.bytes[0]);

    glCheckError();
}
