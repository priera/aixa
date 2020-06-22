#ifndef AIXA_COMMANDBUILDER_H
#define AIXA_COMMANDBUILDER_H

#include "Command.h"

class CommandBuilder {
public:
    virtual ~CommandBuilder() = default;

    virtual CommandCollection buildCommandCollection() = 0;
};

#endif //AIXA_COMMANDBUILDER_H
