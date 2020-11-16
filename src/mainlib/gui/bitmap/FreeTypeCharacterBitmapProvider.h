#ifndef AIXA_FREETYPECHARACTERBITMAPPROVIDER_H
#define AIXA_FREETYPECHARACTERBITMAPPROVIDER_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Bitmap.h"

class FreeTypeCharacterBitmapProvider {
public:
    FreeTypeCharacterBitmapProvider();
    virtual ~FreeTypeCharacterBitmapProvider();

    Bitmap getCharacter(char c);

private:
    FT_Library ft;
    FT_Face face;
};


#endif //AIXA_FREETYPECHARACTERBITMAPPROVIDER_H
