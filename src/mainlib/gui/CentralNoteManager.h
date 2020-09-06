#ifndef AIXA_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
#define AIXA_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H

#include <memory>

#include <mainlib/gui/object/NoteRenderable.h>
#include <mainlib/gui/object/ShadedRenderableObject.h>

#include <mainlib/audio/note/Note.h>

class CharTextureProvider;

class CentralNoteManager :
        public ShadedRenderableObject {
public:
    CentralNoteManager();
    ~CentralNoteManager() override = default;

    void setNewFrontNote(const Note &note) { newFrontChar = getNoteChar(note); }

protected:
    void doMyUpdate() override;

    void applyChildTransformations(RenderableObject *pObject) override;

private:
    std::unique_ptr<NoteRenderable> frontNote;
    std::unique_ptr<NoteRenderable> backNote;

    std::unique_ptr<CharTextureProvider> charTextureProvider;

    float targetAngle;
    char frontChar;
    char newFrontChar;
};


#endif //AIXA_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
