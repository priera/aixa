#include "BitmapsProvider.h"

#include "ImagesProvider.h"

BitmapsProvider::BitmapsProvider() :
    freeTypeProvider(std::make_unique<FreeTypeCharacterBitmapProvider>())
    { }

Bitmap BitmapsProvider::getCharacter(char c) {
    return freeTypeProvider->getCharacter(c);
}

Bitmap BitmapsProvider::getImage(const std::filesystem::path &path) {
    ImagesProvider provider;
    return provider.getImage(path);
}
