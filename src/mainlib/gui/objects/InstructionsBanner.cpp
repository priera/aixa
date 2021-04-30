#include "InstructionsBanner.h"

using namespace std::chrono_literals;

InstructionsBanner::InstructionsBanner(TextureCollection& textureCollection) :
    ShadedRenderableObject(ShadersCollection::Vertex::FRONT_CHARACTER,
                           ShadersCollection::Fragment::CHARACTER,
                           Dimensions{0.9f, 1.125f, 0.1f}),
    textureCollection(&textureCollection), currentInstruction(0) {
    timer = std::make_unique<QTimer>();
    timer->setInterval(2500ms);
    timer->connect(timer.get(), &QTimer::timeout, [this]() { changeInstruction(); });

    instructionsSet = {"drag & drop a wav file", "drag & drop a mp3 file", "ESC to exit"};
}

InstructionsBanner::~InstructionsBanner() { timer->stop(); }

void InstructionsBanner::init() {
    changeInstruction();
    timer->start();
}

void InstructionsBanner::changeInstruction() {
    this->clearChildren();

    const auto& text = instructionsSet[currentInstruction];
    auto instructionTextBox = std::make_shared<ImmutableTextBox>(*this->program, text, FONT_SIZE, 0.5f, 0.5,
                                                                 RATIO, *this->textureCollection);
    addChildObject(0.1, instructionTextBox);

    currentInstruction = ++currentInstruction % instructionsSet.size();
}
