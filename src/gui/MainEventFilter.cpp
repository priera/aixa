#include "MainEventFilter.h"

#include <QApplication>
#include <QKeyEvent>

bool MainEventFilter::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();

        if (key == Qt::Key_Q) {
            auto app = QApplication::instance();
            if (app) app->quit();
        }

        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}
