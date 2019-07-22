#include "mainlib/gui/CentralNoteManager.h"

#include "mainlib/gui/NoteRenderable.h"

CentralNoteManager::CentralNoteManager(const QMatrix4x4 &projectionMatrix) :
    ShadedRenderableObject(projectionMatrix, "./src/mainlib/gui/shaders/vertex.glsl", "./src/mainlib/gui/shaders/fragment.glsl")
    , angle(0)
{
    CharTextureProvider textureProvider;
    auto ch1 = textureProvider.generateChar(' ');
    auto ch2 = textureProvider.generateChar('A');

    frontNote = std::make_unique<NoteRenderable>(ch1, projectionMatrix, *program);
    backNote = std::make_unique<NoteRenderable>(ch2, projectionMatrix, *program);

    addChildObject(0.05, frontNote.get());
    addChildObject(-0.05, backNote.get());
}

void CentralNoteManager::notifyNewValue(const Note &note) {}

void CentralNoteManager::doMyRender() {
    angle -= 0.5;
    if (angle < -360) angle = 0;
}

void CentralNoteManager::applyChildTransformations(RenderableObject *pObject) {
    pObject->rotate(angle);
    float zCord = (pObject == frontNote.get()) ? 0.05 : -0.05;

    pObject->moveCenterAt(0, 0, zCord);
}