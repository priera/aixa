#include "SpectrogramBitmapProvider.h"

void SpectrogramBitmapProvider::sendNewValue(aixa::math::SpectrogramFragment&& fragment) {
    for (auto& slice: fragment.slices) {
        spectrogram.emplace_back(std::move(slice));
    }
}

Bitmap SpectrogramBitmapProvider::buildBitmap() {
    return Bitmap();
}
