#ifndef AIXA_NOTESETTER_H
#define AIXA_NOTESETTER_H

#include <memory>
#include <thread>
#include <atomic>

#include "mainlib/observer/Observable.h"

#include "Note.h"

using NoteNotifier = Observable<Note>;

class NoteSetter : public NoteNotifier {
public:
    NoteSetter();

    ~NoteSetter();

    void setPitch(Note::Pitch pitch);

    void setOctave(int octave);

    void setModifier(Note::Modifier modifier);

private:
    static constexpr int DEFAULT_OCTAVE = 4;

    void workerLoop();
    void mergeNewNote(const Note &incomingNote);

    std::unique_ptr<NotesBuffer> buffer;

    bool noteSeen;

    std::atomic<bool> stopWorker;
    std::thread mergerThread;

    time_t previousPts;
    Note lastNote;
};

#endif //AIXA_NOTESETTER_H
