#include "FreeTypeCharacterBitmapProvider.h"

#include <GL/gl.h>

#include <stdexcept>

FreeTypeCharacterBitmapProvider::FreeTypeCharacterBitmapProvider() {
  if (FT_Init_FreeType(&ft))
    throw std::runtime_error(
        "ERROR::FREETYPE: Could not init FreeType Library");

  if (FT_New_Face(
          ft, "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
          0, &face))
    throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
}

FreeTypeCharacterBitmapProvider::~FreeTypeCharacterBitmapProvider() {
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

Bitmap FreeTypeCharacterBitmapProvider::getCharacter(char c,
                                                     unsigned int pixelSize) {
  const Bitmap *cached = cacheLookup(c, pixelSize);
  if (cached != nullptr) {
    return *cached;
  } else {
    return storeNewGlyph(c, pixelSize);
  }
}

const Bitmap *FreeTypeCharacterBitmapProvider::cacheLookup(
    char c, unsigned int pixelSize) {
  const auto fontSizeIt = cache.find(pixelSize);
  if (fontSizeIt == cache.end()) return nullptr;

  const auto &charCache = fontSizeIt->second;
  const auto charIt = charCache.find(c);
  if (charIt == charCache.end()) return nullptr;

  return &charIt->second;
}

const Bitmap &FreeTypeCharacterBitmapProvider::storeNewGlyph(
    char c, unsigned int pixelSize) {
  FT_Set_Pixel_Sizes(face, 0, pixelSize);

  if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");

  auto buffer = face->glyph->bitmap.buffer;
  auto byteSize = face->glyph->bitmap.rows * face->glyph->bitmap.width;

  auto &metric = metricsStorage.emplace_back(face->glyph->bitmap_top,
                                             face->glyph->bitmap_left,
                                             face->glyph->advance.x);

  const auto &built = cache[pixelSize].emplace(
      c,
      Bitmap{face->glyph->bitmap.rows, face->glyph->bitmap.width, GL_RED,
             std::vector<unsigned char>(buffer, buffer + byteSize), &metric});

  return built.first->second;
}
