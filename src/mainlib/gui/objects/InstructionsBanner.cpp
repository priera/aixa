#include "InstructionsBanner.h"

void InstructionsBanner::init() {
    this->myTextBox = std::make_shared<ImmutableTextBox>(*this->program, "central text", FONT_SIZE, 0.5f, 0.5,
                                                         RATIO, *this->textureCollection);
    addChildObject(0.1, this->myTextBox);
}
