#ifndef AIXA_MAINEVENTFILTER_H
#define AIXA_MAINEVENTFILTER_H

#include <QObject>
#include <unordered_map>

#include <aixa_export.h>
#include <mainlib/Command.h>
#include <mainlib/audio/note/Note.h>
#include <mainlib/audio/note/NoteSetter.h>

class LIB_EXPORT MainEventFilter : public QObject {
    Q_OBJECT
public:
    explicit MainEventFilter(NoteSetter &noteSetter);

    void addCommandsFromCollection(const CommandCollection &commandCollection);
    void removeTransientCommands();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    static void tryToRun(Command *command) {
        if (command) {
            command->execute();
        }
    }

    bool checkNote(int key, Note::Pitch &pitch);
    bool checkOctave(int key, int &octave);
    bool checkModifier(int key, Note::Modifier &modifier);

    Command *volumeUp;
    Command *volumeDown;

    NoteSetter *noteSetter;

    std::unordered_map<int, Note::Pitch> keysToPitchMap;
};

#endif  // AIXA_MAINEVENTFILTER_H
