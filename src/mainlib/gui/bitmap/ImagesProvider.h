#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_IMAGESPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_IMAGESPROVIDER_H

#include <filesystem>

#include "Bitmap.h"

class ImagesProvider {
public:

    Bitmap getImage(const std::filesystem::path &path);
};


#endif //AIXA_SRC_MAINLIB_GUI_BITMAP_IMAGESPROVIDER_H
