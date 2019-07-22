#ifndef ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H

#include "mainlib/audio/note/Note.h"
#include "mainlib/gui/ShadedRenderableObject.h"

class CentralNoteManager :
        public NotesListener,
        public ShadedRenderableObject {
public:
    CentralNoteManager(const QMatrix4x4 &projectionMatrix);
    ~CentralNoteManager() = default;

    void notifyNewValue(const Note &note) override;

protected:
    void doMyRender() override;

private:
    RenderableObject *frontNote;
    RenderableObject *backNote;

    float angle;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
