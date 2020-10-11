#include "mainlib/gui/CentralNoteManager.h"

CentralNoteManager::CentralNoteManager() :
    ShadedRenderableObject("./src/mainlib/gui/shaders/vertex.glsl", "./src/mainlib/gui/shaders/fragment.glsl")
    , targetAngle(-180)
    , frontChar(' ')
    , newFrontChar(' ')
{
    charTextureProvider = std::make_unique<CharTextureProvider>();
    //auto ch1 = charTextureProvider->getChar(' ');
    //auto ch2 = charTextureProvider->getChar('G');

    //frontNote = std::make_unique<NoteRenderable>(ch1, *program);
    //backNote = std::make_unique<NoteRenderable>(ch2,*program);

    //addChildObject(0.05, frontNote.get());
    //addChildObject(-0.05, backNote.get());

}

void CentralNoteManager::beforeRender(const QMatrix4x4 & projectionMatrix) {
    ShadedRenderableObject::beforeRender(projectionMatrix);

    if (frontChar != ' ' && !frontNote) {
        frontNote = std::make_unique<NoteRenderable>(*program);

        addChildObject(0.05, frontNote.get());
    }
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

void CentralNoteManager::applyChildTransformations(RenderableObject *pObject) {
    float zCord = (pObject == frontNote.get()) ? 0.05 : -0.05;

    pObject->moveCenterAt(0, 0, zCord);
}