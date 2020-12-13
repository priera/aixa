#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H

#include <mainlib/math/dft/SpectrogramFragment.h>

#include <QtGui/QColor>
#include <mutex>

#include "Bitmap.h"

class SpectrogramBitmapProvider : public aixa::math::SpectrogramConsumer {
public:
    explicit SpectrogramBitmapProvider(unsigned int height);
    ~SpectrogramBitmapProvider() override = default;

    void sendNewValue(aixa::math::SpectrogramFragment&& fragment) override;

    Bitmap buildBitmap();
    Bitmap getBitmap();

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

    void updateBitmap(const aixa::math::SpectrogramFragment& fragment);
    void fillTexel(std::vector<unsigned char>& vector, unsigned int baseRow, unsigned int baseCol,
                   double sample);
    inline void fillTexel2(unsigned int row, unsigned int col, double sample);

    QColor computeColor(double db);

    inline int project(double value, int min, int max);

    unsigned int height;
    std::shared_ptr<std::vector<unsigned char>> bitmap;

    std::size_t lastColumn;

    std::mutex updateMutex;
    std::vector<std::vector<double>> spectrogram;
    unsigned long rowRepetitions;
};

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_SPECTROGRAMBITMAPPROVIDER_H
