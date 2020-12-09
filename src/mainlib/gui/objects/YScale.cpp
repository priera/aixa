#include "YScale.h"

#include "ImmutableTextBox.h"

void YScale::init() {
    float zPos = 0.05;  // This is actually useless
    for (unsigned int i = 1; i <= STEPS; i++) {
        std::stringstream s;
        float yPos = i * (1.0f / STEPS);
        float hz = i * (scaleMax / STEPS);
        s << hz << " Hz";
        auto textBox = std::make_shared<ImmutableTextBox>(*this->program, s.str(), FONT_SIZE, 0.0f, yPos,
                                                          RATIO, *textureCollection);
        addChildObject(zPos, textBox);
        zPos += 0.01f;
    }
}
