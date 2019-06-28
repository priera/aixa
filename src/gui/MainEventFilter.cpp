#include "MainEventFilter.h"

#include <QApplication>
#include <QKeyEvent>

MainEventFilter::MainEventFilter(const CommandCollection &commandCollection) :
    QObject(),
    volumeUp(nullptr),
    volumeDown(nullptr)
{
    auto it = commandCollection.find("VolumeUp");
    if (it != commandCollection.end()) volumeUp = it->second;

    it = commandCollection.find("VolumeDown");
    if (it != commandCollection.end()) volumeDown = it->second;
}

bool MainEventFilter::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();

        if (key == Qt::Key_Q) {
            auto app = QApplication::instance();
            if (app) app->quit();
        } else if (key == Qt::Key_Up) {
            volumeUp->execute();
        } else if (key == Qt::Key_Down) {
            volumeDown->execute();
        }

        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}
