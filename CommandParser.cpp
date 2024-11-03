#include "CommandParser.h"

#include "CommandExceptions.h"
#include "JsonArray.h"
#include "JsonValue.h"

CommandParser::CommandParser(const std::string &in, const JsonObject &json_object) : CommandLexer(in), json_object(json_object) {
    tokens = lex();
}

JsonValue CommandParser::execute() const {
    return execute_internal(tokens);
}

JsonValue CommandParser::execute_internal(const std::map<long, CommandBit> &tokens) const {
    JsonValue current_value = JsonValue(json_object);

    for (auto &token : tokens) {
        auto alternative = current_value.get();
        if (!std::holds_alternative<JsonObject>(alternative)) {
            throw CommandNotObjectException(token.first);
        }

        JsonObject current_object = std::get<JsonObject>(alternative);
        if (!current_object.get().contains(token.second.name)) {
            throw CommandKeyNotFoundException(token.first);
        }

        current_value = current_object.get()[token.second.name];
        for (auto &subscriptable : token.second.array_indices) {
            // Should be an array
            alternative = current_value.get();
            if (!std::holds_alternative<JsonArray>(alternative)) {
                throw CommandNotArrayException(subscriptable.first);
            }

            JsonArray current_array = std::get<JsonArray>(alternative);
            if (std::holds_alternative<long>(subscriptable.second)) {
                auto index = std::get<long>(subscriptable.second);
                if (index < 0 || index >= current_array.get().size()) {
                    throw CommandArrayIndexOutOfRange(subscriptable.first);
                }

                current_value = current_array.get()[index];
            } else if (std::holds_alternative<std::map<long, CommandBit>>(subscriptable.second)) {
                // Need to parse subscriptable individually
                auto sub_tokens = std::get<std::map<long, CommandBit>>(subscriptable.second);
                const JsonValue sub_value = execute_internal(sub_tokens);

                alternative = sub_value.get();
                if (!std::holds_alternative<long>(alternative)) {
                    throw CommandNotLongException(subscriptable.first);
                }

                auto index = std::get<long>(alternative);
                if (index < 0 || index >= current_array.get().size()) {
                    throw CommandArrayIndexOutOfRange(subscriptable.first);
                }

                current_value = current_array.get()[index];
            }
        }
    }

    return current_value;
}
