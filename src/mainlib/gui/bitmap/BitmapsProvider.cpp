#include "BitmapsProvider.h"

BitmapsProvider::BitmapsProvider() :
    freeTypeProvider(std::make_unique<FreeTypeCharacterBitmapProvider>())
    { }

Bitmap BitmapsProvider::getCharacter(char c) {
    return freeTypeProvider->getCharacter(c);
}
