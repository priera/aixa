#ifndef ALSAPLAYGROUND_ICOMMAND_H
#define ALSAPLAYGROUND_ICOMMAND_H

#include <string>
#include <unordered_map>

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
    virtual std::string getName() const = 0;
};

using CommandCollection = std::unordered_map<std::string, Command *>;

#endif //ALSAPLAYGROUND_ICOMMAND_H
