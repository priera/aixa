#include "NoteConsumer.h"

#include <chrono>
#include <thread>

#include "mainlib/audio/AudioWorker.h"

using namespace std::chrono_literals;

NoteConsumer::NoteConsumer(AudioWorker &worker, NotesBuffer &buffer) :
    worker(&worker),
    buffer(&buffer),
    stopVariable(false),
    noteSeen(false) {}

void NoteConsumer::start() {
    while (!stopVariable) {
        Note n;
        time_t pts;

        if (!buffer->read(n, pts)) {
            std::this_thread::sleep_for(100ms);
            continue;
        }

        mergeNewNote(n);
        auto diff = NotesBuffer::timeDiffInMs(pts, previousPts);

        previousPts = pts;

        double freq = computeFrequency(lastNote);

        std::cout << "Time diff << " << diff << " freq : " << freq << std::endl;

        worker->setFrequency(freq);
    }
}

void NoteConsumer::stop() {
    stopVariable = true;
}

void NoteConsumer::mergeNewNote(const Note &incomingNote) {
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