#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_CHARPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_CHARPROVIDER_H

#include "BitmapProvider.h"

class CharProvider : public BitmapProvider {
public:
    explicit CharProvider(Bitmap bmp);
    virtual ~CharProvider() = default;
};

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_CHARPROVIDER_H
