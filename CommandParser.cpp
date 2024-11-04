#include "CommandParser.h"

#include "CommandExceptions.h"
#include "JsonArray.h"
#include "JsonValue.h"

CommandParser::CommandParser(const std::string &in, const JsonObject &json_object) : CommandLexer(in),
    json_object(json_object) {}

JsonValue CommandParser::execute() const {
    CommandFunc command = lex();
    return execute_internal(command);
}

JsonValue CommandParser::execute_internal(const CommandFunc &command) const {
    if (command.func_type == NONE) {
        // Guaranteed
        return execute_command(std::get<Command>(command.args.begin()->second));
    }

    if (command.func_type == MIN) {
        long double min = 0;
        bool initial = true;

        for (auto &it : command.args) {
            long double value;
            try {
                if (std::holds_alternative<long>(it.second)) {
                    value = std::get<long>(it.second);
                } else if (std::holds_alternative<long double>(it.second)) {
                    value = std::get<long double>(it.second);
                } else {
                    value = execute_internal(std::get<CommandFunc>(it.second)).min();
                }
            } catch (std::invalid_argument &e) {
                throw CommandUnsupportedFunctionCallException(it.first);
            }

            if (initial || value < min) {
                min = value;
                initial = false;
            }
        }

        if (min == static_cast<long>(min)) {
            return JsonValue(static_cast<long>(min));
        }

        return JsonValue(min);
    }

    if (command.func_type == MAX) {
        long double max = 0;
        bool initial = true;

        for (auto &it : command.args) {
            long double value;
            try {
                if (std::holds_alternative<long>(it.second)) {
                    value = std::get<long>(it.second);
                } else if (std::holds_alternative<long double>(it.second)) {
                    value = std::get<long double>(it.second);
                } else {
                    value = execute_internal(std::get<CommandFunc>(it.second)).max();
                }
            } catch (std::invalid_argument &e) {
                throw CommandUnsupportedFunctionCallException(it.first);
            }

            if (initial || value > max) {
                max = value;
                initial = false;
            }
        }

        if (max == static_cast<long>(max)) {
            return JsonValue(static_cast<long>(max));
        }

        return JsonValue(max);
    }

    if (command.func_type == SIZE) {
        if (command.args.size() != 1) {
            throw CommandUnsupportedFunctionCallException(command.args.begin()->first);
        }

        if (!std::holds_alternative<CommandFunc>(command.args.begin()->second)) {
            throw CommandUnsupportedFunctionCallException(command.args.begin()->first);
        }

        JsonValue value = execute_internal(std::get<CommandFunc>(command.args.begin()->second));
        try {
            long double size = value.size();
            if (static_cast<long>(size) == size) {
                return JsonValue(static_cast<long>(size));
            }

            return JsonValue(size);
        } catch (std::invalid_argument &e) {
            throw CommandUnsupportedFunctionCallException(command.args.begin()->first);
        }
    }

    return JsonValue(NULL);     // Impossible
}

JsonValue CommandParser::execute_command(const Command &command) const {
    JsonValue current_value = JsonValue(json_object);

    for (auto &token : command) {
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
            } else if (std::holds_alternative<Box<CommandFunc>>(subscriptable.second)) {
                // Need to parse subscriptable individually
                CommandFunc command_func = std::get<Box<CommandFunc>>(subscriptable.second);
                const JsonValue sub_value = execute_internal(command_func);

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
