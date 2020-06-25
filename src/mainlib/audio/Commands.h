#ifndef AIXA_COMMANDS_H
#define AIXA_COMMANDS_H

#include "mainlib/Command.h"

#include "AudioWorkerOld.h"

class VolumeUp : public Command {
public:
    VolumeUp(AudioWorkerOld & audioWorker) : Command(), worker(&audioWorker) {}

    void execute() override;
    std::string getName() const override;

private:
    AudioWorkerOld *worker;
};


class VolumeDown : public Command {
public:
    VolumeDown(AudioWorkerOld & audioWorker) : Command(), worker(&audioWorker) {}

    void execute() override;
    std::string getName() const override;

private:
    AudioWorkerOld *worker;
};

#endif //AIXA_COMMANDS_H
