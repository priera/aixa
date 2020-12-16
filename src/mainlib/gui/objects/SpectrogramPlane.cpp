#include "SpectrogramPlane.h"

#include <mainlib/gui/gl/ShadersCollection.h>
#include <mainlib/gui/gl/utils.h>

SpectrogramPlane::SpectrogramPlane(SharedBitmapProvider bitmapProvider, const DynamicTexture &texture) :
    ShadedRenderableObject(ShadersCollection::Vertex::TEXTURED_PLANE, ShadersCollection::Fragment::TEXTURE_2D,
                           Dimensions{1.0f, 1.0f, 0.0f}),
    bitmapProvider(std::move(bitmapProvider)),
    texture(texture) {}

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

    glCheckError();
}

void SpectrogramPlane::doMyRender() {
    texture.use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    texture.done();
}
