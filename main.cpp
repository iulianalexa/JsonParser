#include <iostream>
#include <map>
#include <string>
#include <variant>

#include "CommandLexer.h"
#include "CommandParser.h"
#include "JsonArray.h"
#include "JsonObject.h"

int main() {
    //JsonObject obj = JsonObject(R"({"test1": "test2", "test3": {"test4": 5798.45, "test6": ["test10", "test11", false]}})");
    //JsonObject obj = JsonObject(R"({"a": { "b": [ 1, 2, { "c": "test" }, [11, 12] ]}})");
    //std::cout << obj;
    //CommandLexer lexer = CommandLexer("a.b[a.test[55]].c");
    //std::map<long, CommandBit> tokens = lexer.lex();

    JsonObject obj = JsonObject(R"({"a": { "b": [ 1, 2, { "c": "test" }, [11, 12] ]}})");
    auto parser = CommandParser("a.b[a.b[1]].c", obj);
    JsonValue value = parser.execute();
    std::cout << value << "\n";

    parser = CommandParser("a.b[1]", obj);
    value = parser.execute();
    std::cout << value << "\n";

    parser = CommandParser("a.b[2].c", obj);
    value = parser.execute();
    std::cout << value << "\n";

    parser = CommandParser("a.b", obj);
    value = parser.execute();
    std::cout << value << "\n";

    return 0;
}
