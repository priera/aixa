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

bool MainEventFilter::checkNote(int key, Note::Pitch &pitch) {
    auto noteIt = keysToPitchMap.find(key);
    if (noteIt == keysToPitchMap.end()) return false;

    pitch = noteIt->second;
    return true;
}

bool MainEventFilter::checkOctave(int key, int &octave) {
    bool isNumerical = key >= static_cast<int>(Qt::Key_0) && (key < static_cast<int>(Qt::Key_9) + 1);
    if (!isNumerical) return false;

    octave = key - static_cast<int>(Qt::Key_0);
    return true;
}

bool MainEventFilter::checkModifier(int key, Note::Modifier & modifier) {
    if (!(key == Qt::Key_Q || key == Qt::Key_W)) return false;

    modifier = (key == Qt::Key_Q) ? Note::Modifier::FLAT : Note::Modifier::SHARP;
    return true;
}

bool MainEventFilter::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();

        Note::Pitch pitch;
        int octave;
        Note::Modifier modifier;

        if (key == Qt::Key_Escape) {
            auto app = QApplication::instance();
            if (app) app->quit();

        } else if (key == Qt::Key_Up) {
            volumeUp->execute();

        } else if (key == Qt::Key_Down) {
            volumeDown->execute();

        } else if (checkNote(key, pitch)) {
            noteSetter->setPitch(pitch);

        } else if (checkOctave(key, octave)) {
            noteSetter->setOctave(octave);

        } else if (checkModifier(key, modifier)) {
            noteSetter->setModifier(modifier);
        }

        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}
