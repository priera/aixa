#include "ImagesProvider.h"

#include <GL/gl.h>

#include <QtGui/QImage>

Bitmap ImagesProvider::getImage(const std::filesystem::path &path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Image with path " + path.string() + " does not exist");
    }

    auto img = QImage(QString::fromStdString(path.string())).convertToFormat(QImage::Format_RGBA8888);

    auto bitmapData =
        std::make_shared<std::vector<unsigned char>>(img.constBits(), img.constBits() + img.sizeInBytes());
    return {static_cast<unsigned int>(img.height()), static_cast<unsigned int>(img.width()), GL_RGBA,
            bitmapData, nullptr};
}
