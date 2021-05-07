#include "ImagesProvider.h"

#include <GL/gl.h>

#include <QtGui/QImage>

ImagesProvider::ImagesProvider(std::filesystem::path path) : path(std::move(path)) {
    if (!std::filesystem::exists(this->path)) {
        throw std::runtime_error("Image with path " + path.string() + " does not exist");
    }

    auto img = QImage(QString::fromStdString(path.string())).convertToFormat(QImage::Format_RGBA8888);
    auto bitmapData = std::vector<unsigned char>(img.constBits(), img.constBits() + img.sizeInBytes());

    Bitmap bmp = {static_cast<unsigned int>(img.height()), static_cast<unsigned int>(img.width()), GL_RGBA,
                  bitmapData, nullptr};
    this->setBitmap(std::move(bmp));
}
