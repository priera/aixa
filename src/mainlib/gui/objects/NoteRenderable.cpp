#include "NoteRenderable.h"

using namespace std::chrono_literals;

NoteRenderable::NoteRenderable(QOpenGLShaderProgram &program, Dimensions dim) :
    RenderableObject(program, dim), charText(0) {}

void NoteRenderable::updateOnCharData() { angle = 0.0; }

void NoteRenderable::setCharacterText(const unsigned int charTex) {
    std::lock_guard<std::mutex> l(charUpdateMutex);
    this->charText = charTex;

    updateOnCharData();

    Animation::HermiteParams params = {0.25, 0.81, 0.31, 0.0};

    setupAnimation(AnimationParam::ANGLE, 500ms, 20, -180, 0, params);
}

void NoteRenderable::doMyRender() {
    std::lock_guard<std::mutex> l(charUpdateMutex);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const auto textColor = QVector3D(0.7f, 0.7f, 0.7f);
    program->setUniformValue("textColor", textColor);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, charText);
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

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat vertices[] = {
        dim.width, 0.0, 1.0, 1.0, 0.0,       dim.height, 0.0, 0.0, 0.0, 0.0,        0.0, 1.0,

        dim.width, 0.0, 1.0, 1.0, dim.width, dim.height, 1.0, 0.0, 0.0, dim.height, 0.0, 0.0};

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    assert(glGetError() == GL_NO_ERROR);
}

bool NoteRenderable::readyToInitialize() { return charText > 0; }
