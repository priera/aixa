#include "ImmutableTextBox.h"

#include <mainlib/gui/bitmap/GlyphMetrics.h>
#include <mainlib/gui/gl/utils.h>

using namespace std::chrono_literals;

ImmutableTextBox::ImmutableTextBox(QOpenGLShaderProgram &program,
                                   std::string text,
                                   BoxFormat boxFormat,
                                   TextFormat textFormat,
                                   TextureCollection &textureCollection) :
    RenderableObject(program, Dimensions{0.9f, 1.125f, 0.1f}),
    text(std::move(text)), boxFormat(boxFormat), textFormat(textFormat),
    textureCollection(&textureCollection) {
    this->setColor(textFormat.color);
    computeXOffset();
}

void ImmutableTextBox::computeXOffset() {
    xOffset = 0.0;

    if (boxFormat.alignment == Alignment::CENTER) {
        for (auto c : text) {
            auto &texture = textureCollection->getCharacterTexture(c, textFormat.pixelSize);
            const auto &metrics = *(static_cast<GlyphMetrics *>(texture.getBitmap().data.get()));

            xOffset += static_cast<float>(metrics.advanceX >> 6u) * textFormat.textRatio;
        }

        xOffset /= 2;
    }
}

void ImmutableTextBox::init() {
    glGenVertexArrays(1, &vertexAttr);
    glGenBuffers(1, &vertexBuff);

    glBindVertexArray(vertexAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (boxFormat.animateTextColor) {
        Animation::HermiteParams params = {0.42, 0.0, 0.9, 1.0};
        setupAnimation(AnimationParam::COLOR, 2200ms, 30, 0, DEFAULT_COLOR, params,
                       [this](float v) { this->setColor(v); });
    }
}

void ImmutableTextBox::doMyRender() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const auto textColor = QVector3D(color, color, color);
    program->setUniformValue("textColor", textColor);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vertexAttr);

    float xStart = boxFormat.left - xOffset;
    auto ratio = textFormat.textRatio;

    for (auto c : text) {
        auto &texture = textureCollection->getCharacterTexture(c, textFormat.pixelSize);
        const auto &metrics = *(static_cast<GlyphMetrics *>(texture.getBitmap().data.get()));

        float xpos = xStart + (metrics.left * ratio);
        float ypos = boxFormat.top - (metrics.height * ratio - metrics.top * ratio);

        float w = metrics.width * ratio;
        float h = metrics.height * ratio;

        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},     {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f}, {xpos + w, ypos, 1.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 0.0f}};

        texture.use();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuff);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        xStart +=
            (metrics.advanceX >> 6u) * ratio;  // bitshift by 6 to get value in pixels (2^6 = 64 (divide
                                               // amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
}
