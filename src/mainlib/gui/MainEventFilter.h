#ifndef ALSAPLAYGROUND_MAINEVENTFILTER_H
#define ALSAPLAYGROUND_MAINEVENTFILTER_H

#include <QObject>

#include "mainlib/Command.h"

class NoteSetter;

class QEvent;

class MainEventFilter : public QObject {
Q_OBJECT
public:
    MainEventFilter(const CommandCollection &commandCollection, NoteSetter &noteSetter);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    int noteKeys;

    Command *volumeUp;
    Command *volumeDown;

    NoteSetter *noteSetter;
};


#endif //ALSAPLAYGROUND_MAINEVENTFILTER_H
