#include "Texture.h"

Texture::Texture(unsigned int id, Bitmap bmp) : QOpenGLFunctions(), id(id), bitmap(std::move(bmp)) {
    initializeOpenGLFunctions();
}

void Texture::use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);

    update();
}

void Texture::done() { glBindTexture(GL_TEXTURE_2D, 0); }
