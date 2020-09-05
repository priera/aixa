#include "mainlib/gui/CharTextureProvider.h"

#include <set>

#include "mainlib/gui/bitmap/FreeTypeCharacterBitmapProvider.h"

CharTextureProvider::CharTextureProvider() {
    initializeOpenGLFunctions();

    FreeTypeCharacterBitmapProvider provider;

    std::set<char> chars = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (auto &p: chars) {
        auto bitmapData = provider.getCharacter(p);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                bitmapData.width,
                bitmapData.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                bitmapData.buffer
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
                {bitmapData.width, bitmapData.rows},
                {bitmapData.bitmap_left, bitmapData.bitmap_top}
        };

        characters[p] = character;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

CharTextureProvider::Character CharTextureProvider::generateChar(char c) {
    char upperChar = toupper(c);

    auto it = characters.find(upperChar);
    if (it == characters.end())
        return characters[' '];

    return it->second;
}