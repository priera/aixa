#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H

#include <mainlib/math/dft/SpectrogramFragment.h>

#include "Bitmap.h"

class SpectrogramBitmapProvider : public aixa::math::SpectrogramConsumer {
public:

    ~SpectrogramBitmapProvider() override = default;

    void sendNewValue(aixa::math::SpectrogramFragment&& fragment) override;

    Bitmap buildBitmap();

private:
    std::vector<std::vector<double>> spectrogram;
};


#endif //AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
