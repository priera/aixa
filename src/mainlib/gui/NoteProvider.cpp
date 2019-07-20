#include "mainlib/gui/NoteProvider.h"

#include <set>

#include "mainlib/gui/bitmap/FreeTypeCharacterBitmapProvider.h"

NoteProvider::NoteProvider(const QMatrix4x4 &projectionMatrix) :
    projectionMatrix(projectionMatrix)
{
    initializeOpenGLFunctions();

    FreeTypeCharacterBitmapProvider provider;

    std::set<char> pitchChars = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (auto &p: pitchChars) {
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
        NoteRenderable::Character character = {
                texture,
                {bitmapData.width, bitmapData.rows},
                {bitmapData.bitmap_left, bitmapData.bitmap_top}
        };

        characters[p] = character;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

RenderableObject *NoteProvider::generateNote(char note) {
    char upperNote = toupper(note);

    auto it = characters.find(upperNote);
    if (it == characters.end())
        return nullptr;

    auto & ch = it->second;

    return new NoteRenderable(ch, projectionMatrix);
}