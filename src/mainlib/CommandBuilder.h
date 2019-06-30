#ifndef ALSAPLAYGROUND_COMMANDBUILDER_H
#define ALSAPLAYGROUND_COMMANDBUILDER_H

#include "Command.h"

class CommandBuilder {
public:
    virtual ~CommandBuilder() = default;

    virtual CommandCollection buildCommandCollection() = 0;
};

#endif //ALSAPLAYGROUND_COMMANDBUILDER_H
