#include "MainEventFilter.h"

#include <QApplication>
#include <QKeyEvent>

#include "mainlib/audio/note/NoteSetter.h"

MainEventFilter::MainEventFilter(const CommandCollection &commandCollection, NoteSetter &noteSetter) :
    QObject(),
    volumeUp(nullptr),
    volumeDown(nullptr),
    noteSetter(&noteSetter)
{
    keysToPitchMap = {
        {Qt::Key_A, Note::Pitch::A },
        {Qt::Key_B, Note::Pitch::B },
        {Qt::Key_C, Note::Pitch::C },
        {Qt::Key_D, Note::Pitch::D },
        {Qt::Key_E, Note::Pitch::E },
        {Qt::Key_F, Note::Pitch::F },
        {Qt::Key_G, Note::Pitch::G }
    };

    auto it = commandCollection.find("VolumeUp");
    if (it != commandCollection.end()) volumeUp = it->second;

    it = commandCollection.find("VolumeDown");
    if (it != commandCollection.end()) volumeDown = it->second;
}

bool MainEventFilter::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();

        auto noteIt = keysToPitchMap.find(key);
        bool keyIsNote = noteIt != keysToPitchMap.end();

        if (key == Qt::Key_Q) {
            auto app = QApplication::instance();
            if (app) app->quit();

        } else if (key == Qt::Key_Up) {
            volumeUp->execute();

        } else if (key == Qt::Key_Down) {
            volumeDown->execute();

        } else if (keyIsNote) {
            noteSetter->setPitch(noteIt->second);
        }

        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}
