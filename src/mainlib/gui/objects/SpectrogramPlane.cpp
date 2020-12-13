#include "SpectrogramPlane.h"

#include <mainlib/gui/gl/ShadersCollection.h>
#include <mainlib/gui/gl/utils.h>

SpectrogramPlane::SpectrogramPlane(BitmapsProvider &bitmapsProvider) :
    ShadedRenderableObject(ShadersCollection::Vertex::TEXTURED_PLANE, ShadersCollection::Fragment::TEXTURE_2D,
                           Dimensions{1.0f, 1.0f, 0.0f}),
    bitmapsProvider(&bitmapsProvider) {}

SpectrogramPlane::~SpectrogramPlane() noexcept {
    if (isInitialized()) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void SpectrogramPlane::init() {
    std::vector<float> vertices = {
        // positions           // texture coords
        0.0f,      0.0f,       0.0f, 0.0f,  // bottom-left
        dim.width, dim.height, 1.0f, 1.0f,  // top-right
        dim.width, 0.0f,       1.0f, 0.0f,  // bottom-right
        0.0f,      dim.height, 0.0f, 1.0f,  // top-left
    };

    indices = {
        0, 1, 3,  // first triangle
        0, 2, 1   // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto bmp = bitmapsProvider->buildSpectrogram();

    glTexImage2D(GL_TEXTURE_2D, 0, bmp.glStorage, bmp.columns, bmp.rows, 0, bmp.glStorage, GL_UNSIGNED_BYTE,
                 &((*bmp.bytes)[0]));
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    glCheckError();
}

void SpectrogramPlane::doMyRender() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
