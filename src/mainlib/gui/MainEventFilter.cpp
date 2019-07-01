#include "MainEventFilter.h"

#include <QApplication>
#include <QKeyEvent>

#include "mainlib/audio/note/NoteSetter.h"

MainEventFilter::MainEventFilter(const CommandCollection &commandCollection) :
    QObject(),
    volumeUp(nullptr),
    volumeDown(nullptr)
{
    noteKeys = Qt::Key_A | Qt::Key_B | Qt::Key_C | Qt::Key_D | Qt::Key_E | Qt::Key_F | Qt::Key_G;

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

        } /*else if (key & noteKeys) {
            auto steps = key - Qt::Key_A;
            noteSetter->setNote(steps);
        } */

        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}
