#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H

#include <mainlib/math/dft/SpectrogramFragment.h>

#include <QtGui/QColor>
#include <mutex>

#include "Bitmap.h"
#include "BitmapProvider.h"

class SpectrogramBitmapProvider : public aixa::math::SpectrogramConsumer, public BitmapProvider {
public:
    explicit SpectrogramBitmapProvider(unsigned int height);
    ~SpectrogramBitmapProvider() override = default;

    void sendNewValue(aixa::math::SpectrogramFragment&& fragment) override;

    void reset();

private:
    static constexpr double MAX_DB = 60;

    static constexpr int MIN_R = 22;
    static constexpr int MIN_G = 1;
    static constexpr int MIN_B = 56;

    static constexpr int MAX_R = 250;
    static constexpr int MAX_G = 173;
    static constexpr int MAX_B = 20;

    static constexpr unsigned int WIDTH = 768;
    static constexpr auto PIXEL_SIZE = 4 * sizeof(unsigned char);

    inline void fillTexel(unsigned int row, unsigned int col, double sample);

    QColor computeColor(double db);

    inline int project(double value, int min, int max);

    unsigned int height;

    std::vector<unsigned char> bitmapData;
    std::size_t lastColumn;
};

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
