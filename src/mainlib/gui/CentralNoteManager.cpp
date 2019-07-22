#include "mainlib/gui/CentralNoteManager.h"

#include "mainlib/gui/NoteRenderable.h"

CentralNoteManager::CentralNoteManager(const QMatrix4x4 &projectionMatrix) :
    ShadedRenderableObject(projectionMatrix, "./src/mainlib/gui/shaders/vertex.glsl", "./src/mainlib/gui/shaders/fragment.glsl")
    , angle(0)
{
    CharTextureProvider textureProvider;
    auto ch1 = textureProvider.generateChar(' ');
    auto ch2 = textureProvider.generateChar('A');

    frontNote = new NoteRenderable(ch1, projectionMatrix, *program);
    backNote = new NoteRenderable(ch2, projectionMatrix, *program);


}

void CentralNoteManager::notifyNewValue(const Note &note) {}

void CentralNoteManager::doMyRender() {
    angle -= 0.5;
    if (angle < -360) angle = 0;

    frontNote->rotate(angle);
    frontNote->moveCenterAt(0, 0, 0.05);
    frontNote->render();

    backNote->rotate(angle);
    backNote->moveCenterAt(0, 0, -0.05);
    backNote->render();
}