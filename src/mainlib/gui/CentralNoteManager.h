#ifndef ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H

#include <memory>

#include "mainlib/audio/note/Note.h"
#include "mainlib/gui/ShadedRenderableObject.h"

class CharTextureProvider;
class NoteRenderable;

class CentralNoteManager :
        public NotesListener,
        public ShadedRenderableObject {
public:
    CentralNoteManager(const QMatrix4x4 &projectionMatrix);
    ~CentralNoteManager();

    void notifyNewValue(const Note &note) override;

protected:
    void doMyRender() override;

    void applyChildTransformations(RenderableObject *pObject) override;

private:
    std::unique_ptr<NoteRenderable> frontNote;
    std::unique_ptr<NoteRenderable> backNote;

    std::unique_ptr<CharTextureProvider> charTextureProvider;

    float angle;
    float targetAngle;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
