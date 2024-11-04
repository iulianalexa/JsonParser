#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include "CommandLexer.h"
#include "JsonObject.h"
#include "JsonValue.h"

class CommandParser : CommandLexer {
public:
    CommandParser(const std::string &in, const JsonObject &json_object);

    [[nodiscard]] JsonValue execute() const;

private:
    JsonObject json_object;

    [[nodiscard]] JsonValue execute_internal(const CommandFunc &command) const;

    [[nodiscard]] JsonValue execute_command(const Command &command) const;
};

#endif //COMMANDPARSER_H
