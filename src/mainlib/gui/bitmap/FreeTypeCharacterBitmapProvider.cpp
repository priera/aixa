#include "FreeTypeCharacterBitmapProvider.h"

#include <stdexcept>

FreeTypeCharacterBitmapProvider::FreeTypeCharacterBitmapProvider() {
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

    if (FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 80);

}


FreeTypeCharacterBitmapProvider::~FreeTypeCharacterBitmapProvider() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

CharacterBitmapProvider::Character FreeTypeCharacterBitmapProvider::getCharacter(char c) {
    bool blankBuffer = (c == ' ');
    char actualChar = (blankBuffer) ? 'N' : c;

    if (FT_Load_Char(face, actualChar, FT_LOAD_RENDER))
        throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");



    if (blankBuffer) {
        for (int row = 0; row < face->glyph->bitmap.rows; row++) {
            for (int col = 0; col < face->glyph->bitmap.width; col++) {
                face->glyph->bitmap.buffer[row * face->glyph->bitmap.width + col] = 0x0F;
            }
        }
    }

    return { face->glyph->bitmap.rows,
             face->glyph->bitmap.width,
             face->glyph->bitmap_top,
             face->glyph->bitmap_left,
             face->glyph->advance.x,
             face->glyph->bitmap.buffer };
}