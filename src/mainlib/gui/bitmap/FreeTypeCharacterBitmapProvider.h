#ifndef ALSAPLAYGROUND_FREETYPECHARACTERBITMAPPROVIDER_H
#define ALSAPLAYGROUND_FREETYPECHARACTERBITMAPPROVIDER_H

#include "CharacterBitmapProvider.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class FreeTypeCharacterBitmapProvider : public CharacterBitmapProvider {
public:
    FreeTypeCharacterBitmapProvider();
    ~FreeTypeCharacterBitmapProvider();

    Character getCharacter(char c) override;

private:
    FT_Library ft;
    FT_Face face;
};


#endif //ALSAPLAYGROUND_FREETYPECHARACTERBITMAPPROVIDER_H
