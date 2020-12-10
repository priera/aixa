#include "YLogScale.h"

#include <cmath>

#include "ImmutableTextBox.h"

void YLogScale::init() {
    float zPos = 0.05;  // This is actually useless
    float logMax = std::log10(scaleMax) - 1;
    int iterations = logMax;
    int exp = 10;
    for (int i = 0; i <= iterations; i++) {
        std::stringstream s;
        float yPos = static_cast<float>(i) / logMax;
        s << exp << " Hz";
        exp *= 10;

        auto textBox = std::make_shared<ImmutableTextBox>(*this->program, s.str(), FONT_SIZE, 0.0f, yPos,
                                                          RATIO, *textureCollection);
        addChildObject(zPos, textBox);
        zPos += 0.01f;
    }
}
