#include "FreeTypeCharacterBitmapProvider.h"

#include <iostream>
#include <sstream>
#include <exception>

FreeTypeCharacterBitmapProvider::FreeTypeCharacterBitmapProvider() {
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

    if (FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

}


FreeTypeCharacterBitmapProvider::~FreeTypeCharacterBitmapProvider() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

CharacterBitmapProvider::Character FreeTypeCharacterBitmapProvider::getCharacter(char c) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");


    return { face->glyph->bitmap.rows,
             face->glyph->bitmap.width,
             face->glyph->bitmap_top,
             face->glyph->bitmap_left,
             face->glyph->advance.x,
             face->glyph->bitmap.buffer };
}