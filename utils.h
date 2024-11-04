#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <variant>
#include <map>

#define JSON_BEGIN_OBJECT '{'
#define JSON_END_OBJECT '}'
#define JSON_SEPARATOR ','
#define JSON_WHITESPACE ' '
#define JSON_NEWLINE '\n'
#define JSON_BEGIN_ARRAY '['
#define JSON_END_ARRAY ']'
#define JSON_KEY_VALUE_SEPARATOR ':'

#define JSON_TRUE "true"
#define JSON_FALSE "false"
#define JSON_NULL "null"

#define COMMAND_OBJECT_SELECTOR '.'
#define COMMAND_ARRAY_SUBSCRIPT_BEGIN '['
#define COMMAND_ARRAY_SUBSCRIPT_END ']'
#define COMMAND_WHITESPACE ' '

#define COMMAND_FUNCTION_ARGS_SEPARATOR ','
#define COMMAND_FUNCTION_BEGIN '('
#define COMMAND_FUNCTION_END ')'

#define COMMAND_FUNCTION_MAX "max"
#define COMMAND_FUNCTION_MIN "min"
#define COMMAND_FUNCTION_SIZE "size"

class CommandBit;

enum JsonBool {
    JsonTrue, JsonFalse, JsonNull
};

enum JsonSpecialToken {
    BEGIN_OBJECT, END_OBJECT, SEPARATOR, BEGIN_ARRAY, END_ARRAY, KEY_VALUE_SEAPARATOR
};

typedef std::variant<std::string, long, long double, JsonBool, JsonSpecialToken> JsonToken;

typedef std::map<long, CommandBit> Command;

#endif //UTILS_H
