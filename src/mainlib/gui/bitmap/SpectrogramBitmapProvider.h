#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H

#include <QtGui/QColor>

#include <mainlib/math/dft/SpectrogramFragment.h>

#include "Bitmap.h"

class SpectrogramBitmapProvider : public aixa::math::SpectrogramConsumer {
public:

    ~SpectrogramBitmapProvider() override = default;

    void sendNewValue(aixa::math::SpectrogramFragment&& fragment) override;

    Bitmap buildBitmap();

private:
    static constexpr double MAX_DB = 60;

    static constexpr int MIN_R = 22;
    static constexpr int MIN_G = 1;
    static constexpr int MIN_B = 56;

    static constexpr int MAX_R = 250;
    static constexpr int MAX_G = 173;
    static constexpr int MAX_B = 20;

    static constexpr unsigned int WIDTH = 768;
    static constexpr unsigned int COL_REPETITIONS = 1;
    static constexpr auto PIXEL_SIZE = 4 * sizeof(unsigned char);

    void fillTexel(std::vector<unsigned char> &vector, unsigned int baseRow, unsigned int baseCol, double sample);

    QColor computeColor(double db);

    inline int project(double value, int min, int max);

    std::vector<std::vector<double>> spectrogram;
    unsigned int height;
    unsigned long rowRepetitions;
};


#endif //AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
