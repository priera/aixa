#include "NoteSetter.h"

#include <cmath>
#include <cassert>
#include <chrono>

#include "mainlib/audio/note/Note.h"


using namespace std::chrono_literals;

NoteSetter::NoteSetter() :
    buffer(std::make_unique<NotesBuffer>(10)),
    noteSeen(false),
    stopWorker(false) {

    mergerThread = std::thread([this]() {
        workerLoop();
    });
}

NoteSetter::~NoteSetter() {
    stopWorker = true;
    mergerThread.join();
}

void NoteSetter::workerLoop() {
    while (!stopWorker) {
        Note n;
        time_t pts;

        if (!buffer->read(n, pts)) {
            std::this_thread::sleep_for(100ms);
            continue;
        }

        mergeNewNote(n);
        //auto diff = NotesBuffer::timeDiffInMs(pts, previousPts);

        previousPts = pts;

        notifyObservers(lastNote);
    }
}

void NoteSetter::mergeNewNote(const Note &incomingNote) {
    if (!noteSeen) {
        noteSeen = true;
        lastNote = incomingNote;
        if (lastNote.octave == 0)
            lastNote.octave = 4;
        return;
    }

    if (incomingNote.pitch != Note::Pitch::NONE)
        lastNote.pitch = incomingNote.pitch;

    if (incomingNote.octave != 0) {
        lastNote.octave = incomingNote.octave;
    }

    lastNote.modifier = incomingNote.modifier;
}

void NoteSetter::setPitch(Note::Pitch pitch) {
    Note n;
    n.pitch = pitch;

    if (!noteSeen) n.octave = DEFAULT_OCTAVE;

    buffer->insert(n);

    noteSeen = true;
}

void NoteSetter::setOctave(int octave) {
    if (!noteSeen) return;
    if (!(octave >= 0 && octave < 9)) return;

    Note n;
    n.octave = octave;

    buffer->insert(n);
}

void NoteSetter::setModifier(Note::Modifier modifier) {
    if (!noteSeen) return;

    Note n;
    n.modifier = modifier;

    buffer->insert(n);
}