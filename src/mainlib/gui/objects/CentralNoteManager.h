#ifndef AIXA_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
#define AIXA_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H

#include <mainlib/audio/note/Note.h>
#include <mainlib/gui/bitmap/BitmapsProvider.h>
#include <mainlib/gui/objects/NoteRenderable.h>
#include <mainlib/gui/scene/ShadedRenderableObject.h>
#include <mainlib/gui/texture/TextureCollection.h>

#include <memory>

class CentralNoteManager : public ShadedRenderableObject {
public:
    explicit CentralNoteManager(TextureCollection &textureCollection);
    ~CentralNoteManager() override = default;

    void setNewFrontNote(const Note &note) { newFrontChar = getNoteChar(note); }

protected:
    bool readyToInitialize() override;
    void init() override;

    void doMyUpdate() override;

    void applyChildTransformations(RenderableObject &pObject) override;

private:
    static constexpr unsigned int NOTE_CHAR_SIZE = 80;

    TextureCollection *textureCollection;

    std::shared_ptr<NoteRenderable> frontNote;
    std::shared_ptr<NoteRenderable> backNote;

    float targetAngle;
    char frontChar;
    char newFrontChar;
};

#endif  // AIXA_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
