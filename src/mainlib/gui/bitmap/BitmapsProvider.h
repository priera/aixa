#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPSPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPSPROVIDER_H

#include <memory>
#include <filesystem>

#include "Bitmap.h"
#include "FreeTypeCharacterBitmapProvider.h"
#include "SpectrogramBitmapProvider.h"

class BitmapsProvider {
public:
    BitmapsProvider();
    virtual ~BitmapsProvider() = default;

    Bitmap getCharacter(char c);
    Bitmap getImage(const std::filesystem::path &path);
    Bitmap buildProcedural();
    Bitmap buildSpectrogram();

    std::shared_ptr<aixa::math::SpectrogramListener> getSpectrogramListener() {
        return spectrogramProvider;
    }

private:
    std::unique_ptr<FreeTypeCharacterBitmapProvider> freeTypeProvider;
    std::shared_ptr<SpectrogramBitmapProvider> spectrogramProvider;
};


#endif //AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPSPROVIDER_H
