#include "TextureCollection.h"

#include <mainlib/gui/gl/utils.h>

#include <iostream>

TextureCollection::TextureCollection(BitmapBuilders& bitmapBuilders) :
    QOpenGLFunctions(QOpenGLContext::currentContext()->shareContext()), bitmapBuilders(&bitmapBuilders) {
    initializeOpenGLFunctions();
}

Texture& TextureCollection::getCharacterTexture(char c, unsigned int pixelSize) {
    CharKey key = std::make_pair(c, pixelSize);
    const auto& it = charTextures.find(key);
    if (it != charTextures.end()) {
        return it->second;
    }

    charTextures[key] = buildTextureForCharacter(c, pixelSize);
    return charTextures[key];
}

Texture TextureCollection::buildTextureForCharacter(char c, unsigned int pixelSize) {
    GLint previousPixelStore;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousPixelStore);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glCheckError();

    const auto bitmapBuilder = bitmapBuilders->getCharacter(c, pixelSize);
    auto bitmap = bitmapBuilder->get();

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    assert(glGetError() == GL_NO_ERROR);
    glTexImage2D(GL_TEXTURE_2D, 0, bitmap.glStorage, bitmap.columns, bitmap.rows, 0, bitmap.glStorage,
                 GL_UNSIGNED_BYTE, &bitmap.bytes[0]);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, previousPixelStore);

    glFlush();  // make immediately available to the other contexts
    glCheckError();

    return Texture(texId, bitmap);
}

DynamicTexture TextureCollection::buildSpectrogramTexture() {
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const auto bmpBuilder = bitmapBuilders->spectrogram();
    const auto bmp = bmpBuilder->get();

    glTexImage2D(GL_TEXTURE_2D, 0, bmp.glStorage, bmp.columns, bmp.rows, 0, bmp.glStorage, GL_UNSIGNED_BYTE,
                 &bmp.bytes[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

    return DynamicTexture(texId, bmpBuilder);
}
