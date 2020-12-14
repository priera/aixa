#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPBUILDERS_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPBUILDERS_H

#include <filesystem>
#include <memory>

#include "Bitmap.h"
#include "BitmapProvider.h"
#include "FreeTypeCharacterBitmapProvider.h"
#include "SpectrogramBitmapProvider.h"

class BitmapBuilders {
public:
    BitmapBuilders();
    virtual ~BitmapBuilders() = default;

    SharedBitmapProvider getCharacter(char c, unsigned int pixelSize);
    SharedBitmapProvider imageProvider(const std::filesystem::path &path);
    Bitmap buildProcedural();
    SharedBitmapProvider spectrogram();

    std::shared_ptr<aixa::math::SpectrogramConsumer> getSpectrogramConsumer() {
        return spectrogramBitmapProvider;
    }

private:
    std::unique_ptr<FreeTypeCharacterBitmapProvider> freeTypeProvider;
    std::shared_ptr<SpectrogramBitmapProvider> spectrogramBitmapProvider;
};

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPBUILDERS_H
