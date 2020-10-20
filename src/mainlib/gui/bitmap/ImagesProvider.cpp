#include "ImagesProvider.h"

#include <QtGui/QImage>
#include <GL/gl.h>

Bitmap ImagesProvider::getImage(const std::filesystem::path &path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Image with path " + path.string() + " does not exist");
    }

    auto img = QImage(QString::fromStdString(path.string()))
            .convertToFormat(QImage::Format_RGBA8888);

    return {
            static_cast<unsigned int>(img.width()),
            static_cast<unsigned int>(img.height()),
            GL_RGBA,
            std::vector<unsigned char>(img.constBits(), img.constBits() + img.sizeInBytes())
    };
}
