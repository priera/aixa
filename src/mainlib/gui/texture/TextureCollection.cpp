#include "TextureCollection.h"

#include <mainlib/gui/gl/utils.h>

TextureCollection::TextureCollection(BitmapsProvider& bitmapsProvider) :
    QOpenGLFunctions(), bitmapsProvider(&bitmapsProvider) {
    initializeOpenGLFunctions();
}

unsigned int TextureCollection::getCharacterTexture(char c) {
    const auto& it = charTextures.find(c);
    if (it != charTextures.end()) {
        return it->second;
    }

    charTextures[c] = buildTextureForCharacter(c);
    return charTextures[c];
}

unsigned int TextureCollection::buildTextureForCharacter(char c) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glCheckError();

    const auto bitmap = bitmapsProvider->getCharacter(c);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    assert(glGetError() == GL_NO_ERROR);
    glTexImage2D(GL_TEXTURE_2D, 0, bitmap.glStorage, bitmap.columns, bitmap.rows, 0, bitmap.glStorage,
                 GL_UNSIGNED_BYTE, &bitmap.bytes[0]);

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

    return texture;
}
