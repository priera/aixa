#include "SpectrogramBitmapProvider.h"

#include <GL/gl.h>

SpectrogramBitmapProvider::SpectrogramBitmapProvider(unsigned int height) :
    height(height), bitmapData(WIDTH * height * PIXEL_SIZE, 0), lastColumn(0) {
    Bitmap bmp = {height, WIDTH, GL_RGBA, bitmapData, nullptr};

    this->setBitmap(std::move(bmp));
}

void SpectrogramBitmapProvider::sendNewValue(aixa::math::SpectrogramFragment&& fragment) {
    unsigned int col = lastColumn;

    for (const auto& slice : fragment.slices) {
        for (unsigned int i = 0; i < height; i++) {
            auto sample = slice[i];
            fillTexel(i, col, sample);
        }
        col = (col + 1) % WIDTH;
    }

    lastColumn = col;

    // Copying full bitmapData each time is updated in order to prevent some glitches on the render side
    Bitmap bmp = {height, WIDTH, GL_RGBA, bitmapData, nullptr};
    this->setBitmap(std::move(bmp));
}

void SpectrogramBitmapProvider::fillTexel(unsigned int row, unsigned int col, double sample) {
    auto color = computeColor(sample);

    auto rowAddress = row * WIDTH * PIXEL_SIZE;
    const auto colOffset = col * PIXEL_SIZE;
    const auto baseAddress = rowAddress + colOffset;

    bitmapData[baseAddress] = color.red();
    bitmapData[baseAddress + 1] = color.green();
    bitmapData[baseAddress + 2] = color.blue();
    bitmapData[baseAddress + 3] = 255;
}

QColor SpectrogramBitmapProvider::computeColor(double db) {
    double num = (db > MAX_DB) ? MAX_DB : db;
    double ratio = num / MAX_DB;

    int r = project(ratio, MIN_R, MAX_R);
    int g = project(ratio, MIN_G, MAX_G);
    int b = project(ratio, MIN_B, MAX_B);

    return QColor(r, g, b);
}

inline int SpectrogramBitmapProvider::project(double value, int min, int max) {
    return (value * (max - min)) + min;
}
