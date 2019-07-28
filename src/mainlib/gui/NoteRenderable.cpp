#include "mainlib/gui/NoteRenderable.h"

#include <iostream>

NoteRenderable::NoteRenderable(CharTextureProvider::Character &character, QOpenGLShaderProgram &program) :
    RenderableObject(program),
    character(character),
    initialized(false)
{
    updateOnCharData();
}

void NoteRenderable::updateOnCharData() {
    charw = character.size[0];
    charh = character.size[1];

    charPixelRatio = (float)charw / (float)charh;

    w = 1.0;
    h = w / charPixelRatio;
    d = 0.0;
}

void NoteRenderable::setCharacter(const CharTextureProvider::Character & character) {
    std::lock_guard<std::mutex> l(charUpdateMutex);
    this->character = character;

    updateOnCharData();
}

void NoteRenderable::doMyRender() {
    std::lock_guard<std::mutex> l(charUpdateMutex);
    if (!initialized) {
        init();
        initialized = true;
    }

    assert(glGetError() == GL_NO_ERROR);

    program->setUniformValue("textColor", {0.7f, 0.7f, 0.7f });
    assert(glGetError() == GL_NO_ERROR);

    glActiveTexture(GL_TEXTURE0);
    assert(glGetError() == GL_NO_ERROR);

    glBindVertexArray(VAO);
    assert(glGetError() == GL_NO_ERROR);

    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, character.textureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    assert(glGetError() == GL_NO_ERROR);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    assert(glGetError() == GL_NO_ERROR);
}

void NoteRenderable::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    float triangleHeight = w / charPixelRatio;

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