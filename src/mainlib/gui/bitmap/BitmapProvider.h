#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPPROVIDER_H

#include <mutex>

#include "Bitmap.h"

class BitmapProvider {
public:
    virtual ~BitmapProvider() = default;

    Bitmap& get() {
        std::lock_guard l(updateMutex);
        return bitmap;
    };

protected:
    void setBitmap(Bitmap&& bitmap) {
        std::lock_guard l(updateMutex);
        this->bitmap = bitmap;
    }

private:
    std::mutex updateMutex;
    Bitmap bitmap;
};

using SharedBitmapProvider = std::shared_ptr<BitmapProvider>;

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAPPROVIDER_H
