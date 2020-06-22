#ifndef AIXA_MAINEVENTFILTER_H
#define AIXA_MAINEVENTFILTER_H

#include <unordered_map>

#include <QObject>

#include "mainlib/Command.h"

#include "mainlib/audio/note/Note.h"

class NoteSetter;

class QEvent;

class MainEventFilter : public QObject {
Q_OBJECT
public:
    MainEventFilter(const CommandCollection &commandCollection, NoteSetter &noteSetter);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    bool checkNote(int key, Note::Pitch &pitch);
    bool checkOctave(int key, int &octave);
    bool checkModifier(int key, Note::Modifier & modifier);

    Command *volumeUp;
    Command *volumeDown;

    NoteSetter *noteSetter;

    std::unordered_map<int, Note::Pitch> keysToPitchMap;
};


#endif //AIXA_MAINEVENTFILTER_H
