#include "NoteConsumer.h"

#include <chrono>
#include <thread>

#include "mainlib/audio/AudioWorker.h"

using namespace std::chrono_literals;

NoteConsumer::NoteConsumer(AudioWorker &worker, NotesBuffer &buffer) :
    worker(&worker),
    buffer(&buffer),
    stopVariable(false) {}

void NoteConsumer::start() {
    while (!stopVariable) {
        Note n;
        time_t pts;

        if (!buffer->read(n, pts)) {
            std::this_thread::sleep_for(100ms);
            continue;
        }

        auto diff = NotesBuffer::timeDiffInMs(pts, previousPts);
        previousPts = pts;

        double freq = computeFrequency(n);

        worker->setFrequency(freq);

        lastNote = n;
    }
}

void NoteConsumer::stop() {
    stopVariable = true;
}
