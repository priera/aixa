#include "SpectrogramBitmapProvider.h"

#include <iostream>

static int i = 0;

void SpectrogramBitmapProvider::sendNewValue(aixa::math::SpectrogramFragment&& fragment) {
    //TODO: block spectrogram updating when building bitmap
    for (auto& slice: fragment.slices) {
        spectrogram.emplace_back(std::move(slice));
    }

    std::cout << i++ << std::endl;
}

Bitmap SpectrogramBitmapProvider::buildBitmap() {
    return Bitmap();
}
