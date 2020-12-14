#include "DynamicTexture.h"

DynamicTexture::DynamicTexture(unsigned int id, SharedBitmapProvider bitmapProvider) :
    Texture(id, bitmapProvider->get()), bitmapProvider(bitmapProvider) {}

void DynamicTexture::update() {
    this->bitmap = bitmapProvider->get();
    glTexSubImage2D(id, 0, 0, 0, bitmap.columns, bitmap.rows, bitmap.glStorage, GL_UNSIGNED_BYTE,
                    &bitmap.bytes[0]);
}
