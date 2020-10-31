#include "SpectrogramBitmapProvider.h"

#include <iostream>
#include <GL/gl.h>

void SpectrogramBitmapProvider::sendNewValue(aixa::math::SpectrogramFragment&& fragment) {
    //TODO: block spectrogram updating when building bitmap
    for (auto& slice: fragment.slices) {
        spectrogram.emplace_back(std::move(slice));
    }

}

Bitmap SpectrogramBitmapProvider::buildBitmap() {
    //TODO: support sizes not multiple of 256

    rowRepetitions = HEIGHT / spectrogram[0].size();

    std::vector<unsigned char> bytes(WIDTH * HEIGHT * PIXEL_SIZE, 0);
    for (unsigned int i = 0; i < HEIGHT / rowRepetitions; i++) {
        for (unsigned int j = 0; j < WIDTH / COL_REPETITIONS; j++) {
            //Notice that iterating the image in row-major order implies iterating the data in column-major order (cache inefficient!!!)
            auto sample = spectrogram[j][i];
            fillTexel(bytes, i, j, sample);
        }
    }

    return { HEIGHT, WIDTH, GL_RGBA, bytes};
}

void SpectrogramBitmapProvider::fillTexel(std::vector<unsigned char> &bytes,
                                          unsigned int baseRow,
                                          unsigned int baseCol,
                                          double sample) {
    //This isn't, in fact, a good approach. Implies too many cache misses for very few pixels updated
    // Could be suitable for GPUs, nevertheless
    auto color = computeColor(sample);

    auto rowAddress = baseRow * rowRepetitions * WIDTH * PIXEL_SIZE;
    const auto baseColOffset = baseCol * COL_REPETITIONS * PIXEL_SIZE;

    for (auto row = 0; row < rowRepetitions; row++) {
        for (auto col = 0; col < COL_REPETITIONS; col++) {
            const auto colOffset = baseColOffset + (col * PIXEL_SIZE);
            const auto baseAddress = rowAddress + colOffset;

            bytes[baseAddress + col] = color.red();
            bytes[baseAddress + col + 1] = color.green();
            bytes[baseAddress + col + 2] = color.blue();
            bytes[baseAddress + col + 3] = 255;
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
