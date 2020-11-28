#ifndef AIXA_FREETYPECHARACTERBITMAPPROVIDER_H
#define AIXA_FREETYPECHARACTERBITMAPPROVIDER_H

#include <ft2build.h>

#include <unordered_map>
#include FT_FREETYPE_H

#include "Bitmap.h"

class FreeTypeCharacterBitmapProvider {
 public:
  struct GlyphMetrics {
    GlyphMetrics(unsigned int top, unsigned int left, long advanceX)
        : top(top), left(left), advanceX(advanceX) {}

    unsigned int top, left, advanceX;
  };

  FreeTypeCharacterBitmapProvider();
  virtual ~FreeTypeCharacterBitmapProvider();

  Bitmap getCharacter(char c, unsigned int pixelSize = 80);

 private:
  using GlyphCache = std::unordered_map<char, Bitmap>;
  using FontSizeCache = std::unordered_map<unsigned int, GlyphCache>;

  const Bitmap &storeNewGlyph(char c, unsigned int pixelSize);
  const Bitmap *cacheLookup(char c, unsigned int pixelSize);

  FT_Library ft;

  FT_Face face;
  FontSizeCache cache;
  std::vector<GlyphMetrics> metricsStorage;
};

#endif  // AIXA_FREETYPECHARACTERBITMAPPROVIDER_H
