#ifndef ALSAPLAYGROUND_COMMANDS_H
#define ALSAPLAYGROUND_COMMANDS_H

#include "Command.h"

#include "AudioWorker.h"

class VolumeUp : public Command {
public:
    VolumeUp(AudioWorker & audioWorker) : Command(), worker(&audioWorker) {}

    void execute() override;
    std::string getName() const override;

private:
    AudioWorker * worker;
};


class VolumeDown : public Command {
public:
    VolumeDown(AudioWorker & audioWorker) : Command(), worker(&audioWorker) {}

    void execute() override;
    std::string getName() const override;

private:
    AudioWorker * worker;
};

#endif //ALSAPLAYGROUND_COMMANDS_H
