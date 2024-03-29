#include "SceneState.h"

#include <mainlib/globals.h>
#include <mainlib/gui/objects/InstructionsBanner.h>
#include <mainlib/gui/objects/SpectrogramPlane.h>
#include <mainlib/gui/objects/YScale.h>

SceneState::SceneState(Scene &scene, TextureCollection &textureCollection, BitmapBuilders &bitmapBuilders) :
    scene(&scene), textureCollection(&textureCollection), bitmapBuilders(&bitmapBuilders) {}

void SceneState::showUXControls() {
    scene->clear();

    auto instructionsBanner = std::make_shared<InstructionsBanner>(*this->textureCollection);
    scene->add(instructionsBanner);
}

void SceneState::showAudioVisualizations() {
    scene->clear();

    auto spectrogramTexture = this->textureCollection->buildSpectrogramTexture();
    auto spectrogramPlane = new SpectrogramPlane(bitmapBuilders->spectrogram(), spectrogramTexture);
    scene->add(std::shared_ptr<SpectrogramPlane>(spectrogramPlane));

    YScale *yScale_p;
    if (USE_LOG_SCALES) {
        yScale_p = YScale::buildLogarithmic(22050.0f, *textureCollection);
    } else {
        yScale_p = YScale::buildLinear(22050.0f, 10, *textureCollection);
    }

    auto yScale = std::shared_ptr<YScale>(yScale_p);
    scene->add(yScale);
}
