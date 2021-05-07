#include "BitmapBuilders.h"

#include <mainlib/globals.h>

#include "CharProvider.h"
#include "ImagesProvider.h"
#include "ProceduralProvider.h"

BitmapBuilders::BitmapBuilders() :
    freeTypeProvider(std::make_unique<FreeTypeCharacterBitmapProvider>()),
    spectrogramBitmapProvider(std::make_shared<SpectrogramBitmapProvider>(SPECTROGRAM_SIZE)) {}

SharedBitmapProvider BitmapBuilders::getCharacter(char c, unsigned int pixelSize) {
    return std::make_shared<CharProvider>(freeTypeProvider->getCharacter(c, pixelSize));
}

SharedBitmapProvider BitmapBuilders::imageProvider(const std::filesystem::path &path) {
    return std::make_shared<ImagesProvider>(path);
}

Bitmap BitmapBuilders::buildProcedural() { return ProceduralProvider().buildBitmap(); }

SharedBitmapProvider BitmapBuilders::spectrogram() { return spectrogramBitmapProvider; }

void BitmapBuilders::resetSpectrogram() { spectrogramBitmapProvider->reset(); }
