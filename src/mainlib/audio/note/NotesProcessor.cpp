#include "NotesProcessor.h"

#include "mainlib/audio/note/NoteConsumer.h"
#include "mainlib/audio/note/NoteSetter.h"

NotesProcessor::NotesProcessor(AudioWorker &audioWorker) :
    audioWorker(&audioWorker),
    running(false),
    consumer(nullptr),
    consumerThread(nullptr),
    buffer(std::make_unique<NotesBuffer>(10)),
    noteSetter(std::make_unique<NoteSetter>(*buffer)) {
}

NotesProcessor::~NotesProcessor() {}

void NotesProcessor::start() {
    if (running) return;

    auto f = [this]() {
        consumer->start();
    };

    consumer = std::make_unique<NoteConsumer>(*audioWorker, *buffer.get());
    consumerThread = std::unique_ptr<std::thread>(new std::thread(f));

    running = true;
}

void NotesProcessor::stop() {
    if (!running) return;

    consumer->stop();

    consumerThread->join();
    consumerThread = nullptr;

    consumer = nullptr;

    running = false;
}
