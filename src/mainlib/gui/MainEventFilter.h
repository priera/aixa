#ifndef ALSAPLAYGROUND_MAINEVENTFILTER_H
#define ALSAPLAYGROUND_MAINEVENTFILTER_H

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
    Command *volumeUp;
    Command *volumeDown;

    NoteSetter *noteSetter;

    std::unordered_map<int, Note::Pitch> keysToPitchMap;
};


#endif //ALSAPLAYGROUND_MAINEVENTFILTER_H
