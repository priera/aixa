#include "mainlib/gui/NoteRenderable.h"

#include <iostream>

NoteRenderable::NoteRenderable(Character & character, const QMatrix4x4 &projectionMatrix) :
    RenderableObject(projectionMatrix, "./src/mainlib/gui/shaders/vertex.glsl", "./src/mainlib/gui/shaders/fragment.glsl"),
    character(character)
{
    charw = character.size[0];
    charh = character.size[1];

    charPixelRatio = (float)charw / (float)charh;

    w = 1.0;
    h = w / charPixelRatio;

    float triangleHeight = w / charPixelRatio;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat vertices[] = {
        w,   0.0,            1.0, 1.0,
        0.0, triangleHeight, 0.0, 0.0,
        0.0, 0.0,            0.0, 1.0,

        w,   0.0,            1.0, 1.0,
        w,   triangleHeight, 1.0, 0.0,
        0.0, triangleHeight, 0.0, 0.0
    };

    glBufferData(GL_ARRAY_BUFFER,  sizeof(GLfloat) * 6 * 4, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void NoteRenderable::doMyRender() {
    program->setUniformValue("textColor", {0.7f, 0.7f, 0.7f });

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, character.textureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}