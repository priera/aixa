#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPSPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPSPROVIDER_H

#include <memory>
#include <filesystem>

#include "Bitmap.h"
#include "FreeTypeCharacterBitmapProvider.h"

class BitmapsProvider {
public:
    BitmapsProvider();
    virtual ~BitmapsProvider() = default;

    Bitmap getCharacter(char c);
    Bitmap getImage(const std::filesystem::path &path);

private:
    std::unique_ptr<FreeTypeCharacterBitmapProvider> freeTypeProvider;
};


#endif //AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPSPROVIDER_H
