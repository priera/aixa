#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_IMAGESPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_IMAGESPROVIDER_H

#include <filesystem>

#include "Bitmap.h"
#include "BitmapProvider.h"

class ImagesProvider : public BitmapProvider {
public:
    explicit ImagesProvider(std::filesystem::path path);

private:
    std::filesystem::path path;
};

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_IMAGESPROVIDER_H
