#ifndef ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H


#include "mainlib/audio/note/Note.h"

class CentralNoteManager : public NotesListener {
public:
    void notifyNewValue(const Note &note) override;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_GUI_CENTRALNOTEMANAGER_H
