#include "BitmapsProvider.h"

#include "ImagesProvider.h"
#include "ProceduralProvider.h"

BitmapsProvider::BitmapsProvider() :
    freeTypeProvider(std::make_unique<FreeTypeCharacterBitmapProvider>())
    { }

Bitmap BitmapsProvider::getCharacter(char c) {
    return freeTypeProvider->getCharacter(c);
}

Bitmap BitmapsProvider::getImage(const std::filesystem::path &path) {
    return ImagesProvider().getImage(path);
}

Bitmap BitmapsProvider::buildProcedural() {
    return ProceduralProvider().buildBitmap();
}
