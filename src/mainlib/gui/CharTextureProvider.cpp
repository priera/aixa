#include "mainlib/gui/CharTextureProvider.h"

#include <set>
#include <iostream>
#include <mainlib/gui/gl/utils.h>

CharTextureProvider::CharTextureProvider(BitmapsProvider &bitmapsProvider) {
    initializeOpenGLFunctions();

    std::set<char> chars = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glCheckError();

    for (auto &p: chars) {
        auto bitmapData = bitmapsProvider.getCharacter(p);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        assert(glGetError() == GL_NO_ERROR);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                bitmapData.glStorage,
                bitmapData.columns,
                bitmapData.rows,
                0,
                bitmapData.glStorage,
                GL_UNSIGNED_BYTE,
                &bitmapData.bytes[0]
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        // Now store character for later use
        Character character = {
                texture,
                {bitmapData.columns, bitmapData.rows}
        };

        characters[p] = character;
        glCheckError();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();
}

CharTextureProvider::Character &CharTextureProvider::getChar(char c) {
    char upperChar = std::toupper(c);

    auto it = characters.find(upperChar);
    if (it == characters.end())
        return characters[' '];

    return it->second;
}