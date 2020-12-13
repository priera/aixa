#include "BitmapsProvider.h"

#include <mainlib/globals.h>

#include "ImagesProvider.h"
#include "ProceduralProvider.h"

BitmapsProvider::BitmapsProvider() :
    freeTypeProvider(std::make_unique<FreeTypeCharacterBitmapProvider>()),
    spectrogramBitmapProvider(std::make_unique<SpectrogramBitmapProvider>(SPECTROGRAM_SIZE)) {}

Bitmap BitmapsProvider::getCharacter(char c, unsigned int pixelSize) {
    return freeTypeProvider->getCharacter(c, pixelSize);
}

Bitmap BitmapsProvider::getImage(const std::filesystem::path &path) {
    return ImagesProvider().getImage(path);
}

Bitmap BitmapsProvider::buildProcedural() { return ProceduralProvider().buildBitmap(); }

Bitmap BitmapsProvider::buildSpectrogram() { return spectrogramBitmapProvider->buildBitmap(); }
