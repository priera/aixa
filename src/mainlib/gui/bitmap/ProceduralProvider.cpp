#include "ProceduralProvider.h"

#include <GL/gl.h>

Bitmap ProceduralProvider::buildBitmap() {
    /*QImage could had been used also for doing this*/
    const unsigned int width = 512;
    const unsigned int height = 512;
    const auto pixelSize = 4 * sizeof(unsigned char);
    std::vector<unsigned char> bytes(width * height * pixelSize, 0);

    const unsigned char blue = 255;
    const unsigned int redInterval = width / 256;
    const unsigned int greenInterval = height / 256;

    for (unsigned int green = 0; green < 256; green++) {
        for (unsigned int curGreen = 0; curGreen < greenInterval; curGreen++) {
            const auto row = (green * greenInterval) + curGreen;

            for (unsigned int red = 0; red < 256; red++) {
                for (unsigned int curRed = 0; curRed < redInterval; curRed++) {
                    const auto col = (red * redInterval) + curRed;
                    const auto baseAddress = (row * width * pixelSize) + (col * pixelSize);
                    bytes[baseAddress] = red;
                    bytes[baseAddress + 1] = green;
                    bytes[baseAddress + 2] = blue;
                    bytes[baseAddress + 3] = 255;
                }
            }
        }
    }

    return {height, width, GL_RGBA, bytes, nullptr};
}
