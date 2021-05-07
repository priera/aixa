#include "FreeTypeCharacterBitmapProvider.h"

#include <GL/gl.h>

#include <stdexcept>
#include <QStandardPaths>

#include "GlyphMetrics.h"

FreeTypeCharacterBitmapProvider::FreeTypeCharacterBitmapProvider() {
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

    std::string fontPath;
#ifdef WIN32
    fontPath = QStandardPaths::locate(QStandardPaths::StandardLocation::FontsLocation, "consola.ttf").toStdString();
#else
    fontPath = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";
#endif

    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
}

FreeTypeCharacterBitmapProvider::~FreeTypeCharacterBitmapProvider() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Bitmap &FreeTypeCharacterBitmapProvider::getCharacter(char c, unsigned int pixelSize) {
    Bitmap *cached = cacheLookup(c, pixelSize);
    if (cached != nullptr) {
        return *cached;
    } else {
        return storeNewGlyph(c, pixelSize);
    }
}

Bitmap *FreeTypeCharacterBitmapProvider::cacheLookup(char c, unsigned int pixelSize) {
    auto fontSizeIt = cache.find(pixelSize);
    if (fontSizeIt == cache.end())
        return nullptr;

    auto &charCache = fontSizeIt->second;
    auto charIt = charCache.find(c);
    if (charIt == charCache.end())
        return nullptr;

    return &charIt->second;
}

Bitmap &FreeTypeCharacterBitmapProvider::storeNewGlyph(char c, unsigned int pixelSize) {
    FT_Set_Pixel_Sizes(face, 0, pixelSize);

    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load Glyph");

    auto buffer = face->glyph->bitmap.buffer;
    auto byteSize = face->glyph->bitmap.rows * face->glyph->bitmap.width;

    auto metrics = std::make_shared<GlyphMetrics>(face->glyph->bitmap_top, face->glyph->bitmap_left,
                                                  face->glyph->bitmap.width, face->glyph->bitmap.rows,
                                                  face->glyph->advance.x);

    auto bitmapData = std::vector<unsigned char>(buffer, buffer + byteSize);
    const auto &built = cache[pixelSize].emplace(
        c, Bitmap{face->glyph->bitmap.rows, face->glyph->bitmap.width, GL_RED, bitmapData, metrics});

    return built.first->second;
}
