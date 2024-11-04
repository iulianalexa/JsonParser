#ifndef COMMANDFUNC_H
#define COMMANDFUNC_H
#include <map>
#include <variant>

#include "utils.h"
#include "CommandBit.h"

enum CommandFuncType {
    NONE, MIN, MAX, SIZE
};

class CommandFunc {
public:
    CommandFunc(const std::map<long, std::variant<CommandFunc, Command, long, long double>> &args, const CommandFuncType &func_type);

private:
    friend class CommandParser;

    std::map<long, std::variant<CommandFunc, Command, long, long double>> args;
    CommandFuncType func_type;
};

#endif //COMMANDFUNC_H