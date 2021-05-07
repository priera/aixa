#include "CentralNoteManager.h"

#include <mainlib/gui/gl/ShadersCollection.h>

CentralNoteManager::CentralNoteManager(TextureCollection &textureCollection) :
    ShadedRenderableObject(ShadersCollection::Vertex::TEXTURED_PLANE, ShadersCollection::Fragment::CHARACTER,
                           Dimensions{0.9f, 1.125f, 0.1f}),
    textureCollection(&textureCollection),
    targetAngle(-180),
    frontChar(' '),
    newFrontChar(' ') {}

bool CentralNoteManager::readyToInitialize() { return frontChar != ' ' && !frontNote; }

void CentralNoteManager::init() {
    frontNote = std::make_shared<NoteRenderable>(*program, Dimensions{dim.width, dim.height, 0.0f});

    addChildObject(0.05, frontNote);
}

void CentralNoteManager::doMyUpdate() {
    RenderableObject::doMyUpdate();

    if (frontChar != newFrontChar) {
        frontChar = newFrontChar;

        if (frontNote) {
            auto charTex = textureCollection->getCharacterTexture(frontChar, NOTE_CHAR_SIZE);
            frontNote->setCharacterText(charTex);
        }
    }
}

void CentralNoteManager::applyChildTransformations(RenderableObject &pObject) {
    float zCord = (&pObject == frontNote.get()) ? 0.05 : -0.05;

    pObject.moveCenterAt(0, 0, zCord);
}
