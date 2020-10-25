#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H

#include <mainlib/math/dft/SpectrogramFragment.h>

#include "Bitmap.h"

class SpectrogramBitmapProvider : public aixa::math::SpectrogramListener {
public:

    ~SpectrogramBitmapProvider() override = default;

    void notifyNewValue(const aixa::math::SpectrogramFragment& e) override;

    Bitmap buildBitmap();
};


#endif //AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
