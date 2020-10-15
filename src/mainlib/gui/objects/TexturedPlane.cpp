#include <QtGui/QImage>
#include <iostream>
#include <mainlib/gui/gl/utils.h>
#include "TexturedPlane.h"

TexturedPlane::TexturedPlane(std::filesystem::path texturePath) :
    ShadedRenderableObject("./shaders/textured_plane.vert",
                           "./shaders/2d_texture.frag",
                           Dimensions{1.0f, 1.0f, 0.0f}),
    texturePath(std::move(texturePath)) {

    if (!std::filesystem::exists(this->texturePath)) {
        throw std::runtime_error("Image with path " + this->texturePath.string() + " does not exist");
    }
}

TexturedPlane::~TexturedPlane() noexcept {
    if (isInitialized()) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void TexturedPlane::init() {
    const float halfWidth = dim.width / 2;
    const float halfHeight = dim.height / 2;
    std::vector<float> vertices = {
            // positions   // texture coords
            -halfWidth, -halfHeight,  0.0f, 0.0f, // bottom-left
             halfWidth,  halfHeight,  1.0f, 1.0f, // top-right
             halfWidth, -halfHeight,  1.0f, 0.0f, // bottom-right
            -halfWidth,  halfHeight,  0.0f, 1.0f, // top-left
    };

    indices = {
            0, 1, 3, // first triangle
            0, 2, 1  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    QImage original(QString::fromStdString(texturePath.string()));
    auto textureImage = new QImage(original.convertToFormat(QImage::Format_RGB888));

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 textureImage->width(),
                 textureImage->height(),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 textureImage->constBits());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    glCheckError();
}

void TexturedPlane::doMyRender() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

