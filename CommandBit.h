#ifndef COMMANDBIT_H
#define COMMANDBIT_H
#include <map>
#include <string>
#include <variant>

class CommandBit {
    friend class CommandLexer;
    friend class CommandParser;

    CommandBit(std::string name, const std::map<long, std::variant<std::map<long, CommandBit>, long>> &array_indices);

    std::map<long, std::variant<std::map<long, CommandBit>, long>> array_indices;
    std::string name;
};

#endif //COMMANDBIT_H
