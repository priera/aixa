#include "InstructionsBanner.h"

using namespace std::chrono_literals;

InstructionsBanner::InstructionsBanner(TextureCollection& textureCollection) :
    ShadedRenderableObject(ShadersCollection::Vertex::FRONT_CHARACTER,
                           ShadersCollection::Fragment::CHARACTER,
                           Dimensions{0.9f, 1.125f, 0.1f}),
    textureCollection(&textureCollection), currentInstruction(0) {
    timer = std::make_unique<QTimer>();
    timer->setInterval(4600ms);
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
    auto boxFormat = ImmutableTextBox::BoxFormat{0.5, 0.5, true, ImmutableTextBox::Alignment::CENTER};
    auto textFormat = ImmutableTextBox::TextFormat{FONT_SIZE, RATIO, 0.0};
    auto instructionTextBox = std::make_shared<ImmutableTextBox>(*this->program, text, boxFormat, textFormat,
                                                                 *this->textureCollection);
    addChildObject(0.1, instructionTextBox);

    currentInstruction = ++currentInstruction % instructionsSet.size();
}
