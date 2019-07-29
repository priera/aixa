#include "mainlib/gui/CentralNoteManager.h"

#include "mainlib/gui/NoteRenderable.h"

CentralNoteManager::CentralNoteManager() :
    ShadedRenderableObject("./src/mainlib/gui/shaders/vertex.glsl", "./src/mainlib/gui/shaders/fragment.glsl")
    , angle(0)
    , targetAngle(-180)
    , frontChar(' ')
    , newFrontChar(' ')
{
    charTextureProvider = std::make_unique<CharTextureProvider>();
    //auto ch1 = charTextureProvider->generateChar(' ');
    //auto ch2 = charTextureProvider->generateChar('G');

    //frontNote = std::make_unique<NoteRenderable>(ch1, *program);
    //backNote = std::make_unique<NoteRenderable>(ch2,*program);

    //addChildObject(0.05, frontNote.get());
    //addChildObject(-0.05, backNote.get());

}

CentralNoteManager::~CentralNoteManager() { }

void CentralNoteManager::notifyNewValue(const Note &note) {
    newFrontChar = getNoteChar(note);
}

void CentralNoteManager::doMyUpdate(){
    if (frontChar != newFrontChar) {

        frontChar = newFrontChar;

        auto charText = charTextureProvider->generateChar(frontChar);
        if (!frontNote) {
            frontNote = std::make_unique<NoteRenderable>(charText, *program);

            addChildObject(0.05, frontNote.get());
        }

        frontNote->setCharacter(charText);
    }

    //if (angle > targetAngle)
    angle -= 0.5;
}

void CentralNoteManager::applyChildTransformations(RenderableObject *pObject) {
    pObject->rotate(angle);
    float zCord = (pObject == frontNote.get()) ? 0.05 : -0.05;

    pObject->moveCenterAt(0, 0, zCord);
}