#ifndef ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H

#include "mainlib/audio/note/Note.h"
#include "mainlib/gui/RenderableObject.h"

class CentralNoteManager :
        public NotesListener,
        public RenderableObject {
public:
    CentralNoteManager(const QMatrix4x4 &projectionMatrix);

    void notifyNewValue(const Note &note) override;

protected:
    void doMyRender() override;

private:
    RenderableObject *frontNote;
    RenderableObject *backNote;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
