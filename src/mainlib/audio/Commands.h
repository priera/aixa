#ifndef AIXA_COMMANDS_H
#define AIXA_COMMANDS_H

#include "mainlib/Command.h"

#include "AudioWorker.h"

class VolumeUp : public Command {
public:
    VolumeUp(AudioWorker & audioWorker) : Command(), worker(&audioWorker) {}

    void execute() override;
    std::string getName() const override;

private:
    AudioWorker *worker;
};


class VolumeDown : public Command {
public:
    VolumeDown(AudioWorker & audioWorker) : Command(), worker(&audioWorker) {}

    void execute() override;
    std::string getName() const override;

private:
    AudioWorker *worker;
};

#endif //AIXA_COMMANDS_H
