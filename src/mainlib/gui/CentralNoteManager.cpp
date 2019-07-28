#include "mainlib/gui/CentralNoteManager.h"

#include "mainlib/gui/NoteRenderable.h"

CentralNoteManager::CentralNoteManager() :
    ShadedRenderableObject("./src/mainlib/gui/shaders/vertex.glsl", "./src/mainlib/gui/shaders/fragment.glsl")
    , angle(0)
    , targetAngle(-180)
{
    charTextureProvider = std::make_unique<CharTextureProvider>();
    auto ch1 = charTextureProvider->generateChar('B');
    auto ch2 = charTextureProvider->generateChar('A');

    frontNote = std::make_unique<NoteRenderable>(ch1, *program);
    backNote = std::make_unique<NoteRenderable>(ch2,*program);

    addChildObject(0.05, frontNote.get());
    addChildObject(-0.05, backNote.get());
}

CentralNoteManager::~CentralNoteManager() { }

void CentralNoteManager::notifyNewValue(const Note &note) {
    auto c = getNoteChar(note);

    std::cout << "hola" << static_cast<int>(note.pitch) << " " << c << std::endl;

    auto charText = charTextureProvider->generateChar(c);
    frontNote->setCharacter(charText);
}

void CentralNoteManager::doMyUpdate(){
    //if (angle > targetAngle)
        angle -= 0.5;
}

void CentralNoteManager::applyChildTransformations(RenderableObject *pObject) {
    pObject->rotate(angle);
    float zCord = (pObject == frontNote.get()) ? 0.05 : -0.05;

    pObject->moveCenterAt(0, 0, zCord);
}