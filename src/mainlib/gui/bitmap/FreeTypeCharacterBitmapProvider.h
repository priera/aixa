#ifndef AIXA_FREETYPECHARACTERBITMAPPROVIDER_H
#define AIXA_FREETYPECHARACTERBITMAPPROVIDER_H

#include <ft2build.h>

#include <unordered_map>
#include FT_FREETYPE_H

#include "Bitmap.h"

class FreeTypeCharacterBitmapProvider {
public:
    FreeTypeCharacterBitmapProvider();
    virtual ~FreeTypeCharacterBitmapProvider();

    Bitmap &getCharacter(char c, unsigned int pixelSize);

private:
    using GlyphCache = std::unordered_map<char, Bitmap>;
    using FontSizeCache = std::unordered_map<unsigned int, GlyphCache>;

    Bitmap &storeNewGlyph(char c, unsigned int pixelSize);
    Bitmap *cacheLookup(char c, unsigned int pixelSize);

    FT_Library ft;

    FT_Face face;
    FontSizeCache cache;
};

#endif  // AIXA_FREETYPECHARACTERBITMAPPROVIDER_H
