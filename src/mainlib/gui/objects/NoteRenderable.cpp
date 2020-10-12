#include "NoteRenderable.h"

using namespace std::chrono_literals;

NoteRenderable::NoteRenderable(QOpenGLShaderProgram &program) :
    RenderableObject(program),
    character(nullptr) { }

void NoteRenderable::updateOnCharData() {
    float charw = character->size[0];
    float charh = character->size[1];

    charPixelRatio = charw / charh;

    w = 1.0;
    h = w / charPixelRatio;
    d = 0.0;
    angle = 0.0;
}

void NoteRenderable::setCharacter(const CharTextureProvider::Character &charTex) {
    std::lock_guard<std::mutex> l(charUpdateMutex);
    this->character = &charTex;

    updateOnCharData();

    Animation::HermiteParams params = {
        0.25, 0.81,
        0.31, 0.0
    };

    setupAnimation(AnimationParam::ANGLE, 500ms, 20, -180, 0, params);
}

void NoteRenderable::doMyRender() {
    std::lock_guard<std::mutex> l(charUpdateMutex);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    program->setUniformValue("textColor", {0.7f, 0.7f, 0.7f });

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, character->textureID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);

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

    glBufferData(GL_ARRAY_BUFFER,  sizeof(GLfloat) * 6 * 4, vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    assert(glGetError() == GL_NO_ERROR);
}

bool NoteRenderable::readyToInitialize() {
    return character;
}
