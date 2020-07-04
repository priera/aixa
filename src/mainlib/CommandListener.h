#ifndef AIXA_COMMANDBUILDER_H
#define AIXA_COMMANDBUILDER_H

#include "Command.h"

class CommandListener {
public:
    virtual ~CommandListener() = default;

    virtual CommandCollection getCommandCollection() = 0;
};

#endif //AIXA_COMMANDBUILDER_H
