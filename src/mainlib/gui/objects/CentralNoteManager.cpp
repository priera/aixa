#include "CentralNoteManager.h"

#include <mainlib/gui/gl/ShadersCollection.h>

CentralNoteManager::CentralNoteManager(BitmapsProvider &bitmapsProvider) :
    ShadedRenderableObject(ShadersCollection::VERTEX_TEXTURED_PLANE,
                           ShadersCollection::FRAG_CHARACTER,
                           Dimensions{0.9f, 1.125f, 0.1f})
    , targetAngle(-180)
    , frontChar(' ')
    , newFrontChar(' ')
{
    charTextureProvider = std::make_unique<CharTextureProvider>(bitmapsProvider);
    //auto ch1 = charTextureProvider->getChar(' ');
    //auto ch2 = charTextureProvider->getChar('G');

    //frontNote = std::make_unique<NoteRenderable>(ch1, *program);
    //backNote = std::make_unique<NoteRenderable>(ch2,*program);

    //addChildObject(0.05, frontNote.get());
    //addChildObject(-0.05, backNote.get());

}

bool CentralNoteManager::readyToInitialize() {
    return frontChar != ' ' && !frontNote;
}

void CentralNoteManager::init() {
    frontNote = std::make_shared<NoteRenderable>(*program, Dimensions{dim.width, dim.height, 0.0f});

    addChildObject(0.05, frontNote);
}

void CentralNoteManager::doMyUpdate(){
    RenderableObject::doMyUpdate();

    if (frontChar != newFrontChar) {
        frontChar = newFrontChar;

        if (frontNote) {
            auto &charTex = charTextureProvider->getChar(frontChar);
            frontNote->setCharacter(charTex);
        }
    }
}

void CentralNoteManager::applyChildTransformations(RenderableObject &pObject) {
    float zCord = (&pObject == frontNote.get()) ? 0.05 : -0.05;

    pObject.moveCenterAt(0, 0, zCord);
}

