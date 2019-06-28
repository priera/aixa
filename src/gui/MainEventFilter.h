#ifndef ALSAPLAYGROUND_MAINEVENTFILTER_H
#define ALSAPLAYGROUND_MAINEVENTFILTER_H

#include <QObject>

#include "Command.h"

class QEvent;

class MainEventFilter : public QObject {
Q_OBJECT
public:
    MainEventFilter(const CommandCollection & commandCollection);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Command *volumeUp;
    Command *volumeDown;
};


#endif //ALSAPLAYGROUND_MAINEVENTFILTER_H
