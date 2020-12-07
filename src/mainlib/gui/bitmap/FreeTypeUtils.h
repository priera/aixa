#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_FREETYPEUTILS_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_FREETYPEUTILS_H

#include <ft2build.h>
#include FT_FREETYPE_H

namespace FreeTypeUtils {

void printCharacter(const FT_Bitmap &glyph);
}

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_FREETYPEUTILS_H
