#include "SpectrogramBitmapProvider.h"

#include <GL/gl.h>

SpectrogramBitmapProvider::SpectrogramBitmapProvider(unsigned int height) :
    height(height),
    bitmap(std::make_shared<std::vector<unsigned char>>(WIDTH * height * PIXEL_SIZE, 0)),
    lastColumn(0) {}

void SpectrogramBitmapProvider::sendNewValue(aixa::math::SpectrogramFragment&& fragment) {
    std::unique_lock l(updateMutex);
    updateBitmap(fragment);
}

void SpectrogramBitmapProvider::updateBitmap(const aixa::math::SpectrogramFragment& fragment) {
    const std::size_t slicesCount = fragment.slices.size();
    for (const auto& slice : fragment.slices) {
        unsigned int startCol = lastColumn;
        for (std::size_t j = 0; j < slicesCount; j++) {
            unsigned int col = (startCol + j) % WIDTH;
            for (unsigned int i = 0; i < height; i++) {
                auto sample = slice[i];
                fillTexel2(i, col, sample);
            }
        }
    }

    lastColumn = (lastColumn + slicesCount) % WIDTH;
}

Bitmap SpectrogramBitmapProvider::getBitmap() {
    std::unique_lock l(updateMutex);

    return {height, WIDTH, GL_RGBA, bitmap, nullptr};
}

void SpectrogramBitmapProvider::fillTexel2(unsigned int row, unsigned int col, double sample) {
    auto color = computeColor(sample);

    auto rowAddress = row * WIDTH * PIXEL_SIZE;
    const auto colOffset = col * PIXEL_SIZE;
    const auto baseAddress = rowAddress + colOffset;

    (*bitmap)[baseAddress] = color.red();
    (*bitmap)[baseAddress + 1] = color.green();
    (*bitmap)[baseAddress + 2] = color.blue();
    (*bitmap)[baseAddress + 3] = 255;
}

Bitmap SpectrogramBitmapProvider::buildBitmap() {
    // TODO: support sizes not multiple of 256
    height = spectrogram[0].size();
    rowRepetitions = 1;

    auto bitmapData = std::make_shared<std::vector<unsigned char>>(WIDTH * height * PIXEL_SIZE, 0);
    for (unsigned int i = 0; i < height / rowRepetitions; i++) {
        for (unsigned int j = 0; j < WIDTH / COL_REPETITIONS; j++) {
            // Notice that iterating the image in row-major order implies iterating the data in column-major
            // order (cache inefficient!!!)
            auto sample = spectrogram[j][i];
            fillTexel(*bitmapData, i, j, sample);
        }
    }

    return {height, WIDTH, GL_RGBA, bitmapData, nullptr};
}

void SpectrogramBitmapProvider::fillTexel(std::vector<unsigned char>& bytes, unsigned int baseRow,
                                          unsigned int baseCol, double sample) {
    // This isn't, in fact, a good approach. Implies too many cache misses for very few pixels updated
    // Could be suitable for GPUs, nevertheless
    auto color = computeColor(sample);

    auto rowAddress = baseRow * rowRepetitions * WIDTH * PIXEL_SIZE;
    const auto baseColOffset = baseCol * COL_REPETITIONS * PIXEL_SIZE;

    for (auto row = 0; row < rowRepetitions; row++) {
        for (auto col = 0; col < COL_REPETITIONS; col++) {
            const auto colOffset = baseColOffset + (col * PIXEL_SIZE);
            const auto baseAddress = rowAddress + colOffset;

            bytes[baseAddress] = color.red();
            bytes[baseAddress + 1] = color.green();
            bytes[baseAddress + 2] = color.blue();
            bytes[baseAddress + 3] = 255;
        }
        rowAddress += WIDTH * PIXEL_SIZE;
    }
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
