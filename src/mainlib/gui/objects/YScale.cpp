#include "YScale.h"

#include <cmath>

#include "ImmutableTextBox.h"

YScale* YScale::buildLinear(float scaleMax, unsigned int steps, TextureCollection& textureCollection) {
    ScaleContent content;
    for (unsigned int i = 1; i <= steps; i++) {
        std::stringstream s;
        float yPos = i * (1.0f / steps);
        float hz = i * (scaleMax / steps);
        s << hz << " Hz";

        content.emplace_back(std::make_pair(yPos, s.str()));
    }

    return new YScale(content, textureCollection);
}

YScale* YScale::buildLogarithmic(float scaleMax, TextureCollection& textureCollection) {
    ScaleContent content;

    float logMax = std::log10(scaleMax) - 1;
    int iterations = logMax;
    int exp = 10;
    for (int i = 0; i <= iterations; i++) {
        std::stringstream s;
        float yPos = static_cast<float>(i) / logMax;
        s << exp << " Hz";
        exp *= 10;

        content.emplace_back(std::make_pair(yPos, s.str()));
    }

    return new YScale(content, textureCollection);
}

void YScale::init() {
    float zPos = 0.05;  // This is actually useless

    for (auto& content : scaleContent) {
        float yPos = content.first;
        std::string text = std::move(content.second);
        auto boxFormat = ImmutableTextBox::BoxFormat{0.0, yPos};
        auto textFormat = ImmutableTextBox::TextFormat{FONT_SIZE, RATIO};
        auto textBox = std::make_shared<ImmutableTextBox>(*this->program, text, boxFormat, textFormat,
                                                          *this->textureCollection);

        addChildObject(zPos, textBox);
        zPos += 0.01f;
    }
}
