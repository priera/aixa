#ifndef AIXA_ICOMMAND_H
#define AIXA_ICOMMAND_H

#include <string>
#include <unordered_map>

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
    virtual std::string getName() const = 0;
};

//FIXME: memory leaks!!!!
using CommandCollection = std::unordered_map<std::string, Command *>;

#endif //AIXA_ICOMMAND_H
