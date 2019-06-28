#ifndef ALSAPLAYGROUND_MAINEVENTFILTER_H
#define ALSAPLAYGROUND_MAINEVENTFILTER_H

#include <QObject>

class QEvent;

class MainEventFilter : public QObject {
Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};


#endif //ALSAPLAYGROUND_MAINEVENTFILTER_H
