#include "FreeTypeCharacterBitmapProvider.h"

#include <GL/gl.h>
#include <stdexcept>

FreeTypeCharacterBitmapProvider::FreeTypeCharacterBitmapProvider() {
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

    if (FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
}


FreeTypeCharacterBitmapProvider::~FreeTypeCharacterBitmapProvider() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Bitmap FreeTypeCharacterBitmapProvider::getCharacter(char c, unsigned int pixelSize) {
    bool blankBuffer = (c == ' ');
    char actualChar = (blankBuffer) ? 'N' : c;

    FT_Set_Pixel_Sizes(face, 0, pixelSize);

    if (FT_Load_Char(face, actualChar, FT_LOAD_RENDER))
        throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");


    if (blankBuffer) {
        for (int row = 0; row < face->glyph->bitmap.rows; row++) {
            for (int col = 0; col < face->glyph->bitmap.width; col++) {
                face->glyph->bitmap.buffer[row * face->glyph->bitmap.width + col] = 0x0F;
            }
        }
    }

    auto buffer = face->glyph->bitmap.buffer;
    auto byteSize = face->glyph->bitmap.rows * face->glyph->bitmap.width;

    return {face->glyph->bitmap.rows,
            face->glyph->bitmap.width,
            GL_RED,
            std::vector<unsigned char>(buffer, buffer + byteSize)};
}