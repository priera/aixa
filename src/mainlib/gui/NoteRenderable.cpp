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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,  sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void NoteRenderable::doMyRender() {
    program->setUniformValue("textColor", {0.5, 0.8f, 0.2f });

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float triangleHeight = w / charPixelRatio;

    GLfloat vertices[6][4] = {
            { w, 0.0,               1.0, 1.0 },
            { 0.0, triangleHeight,  0.0, 0.0 },
            { 0.0, 0.0,             0.0, 1.0 },

            { w, 0.0,               1.0, 1.0 },
            { w, triangleHeight,    1.0, 0.0 },
            { 0.0, triangleHeight,  0.0, 0.0 }
    };

    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, character.textureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}