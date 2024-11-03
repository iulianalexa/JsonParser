#include "JsonObject.h"

#include "JsonExceptions.h"
#include "Json.h"

JsonObject::JsonObject(const std::string &in) : JsonLexer(in) {
    parse();
}

std::ostream &operator<<(std::ostream &os, const JsonObject &json_object) {
    const auto data = json_object.get();
    os << "{";
    const int map_size = data.size();
    int cnt = 0;

    for (const auto &iter : data) {
        os << '"' << iter.first << '"' << ": " << iter.second;
        cnt++;

        if (cnt < map_size) {
            os << ", ";
        }
    }

    os << "}";

    return os;
}

std::map<std::string, JsonValue> JsonObject::get() const {
    return data;
}

void JsonObject::parse() {
    std::map<int, JsonToken> tokens = lex();

    if (tokens.empty()) {
        throw JsonEmptyStringException();
    }

    if (!std::holds_alternative<JsonSpecialToken>(tokens[0])) {
        throw JsonUnexpectedTokenException(0);
    }

    auto tok = std::get<JsonSpecialToken>(tokens[0]);

    if (tok != BEGIN_OBJECT) {
        throw JsonUnexpectedTokenException(0);
    }

    auto iter = tokens.cbegin();
    auto end = tokens.cend();
    ++iter;
    parse_json_object(iter, end);
    if (iter != end) {
        throw JsonUnexpectedTokenException(iter->first);
    }
}

void JsonObject::parse_json_object_pair(std::map<int, JsonToken>::const_iterator &iter, const std::map<int, JsonToken>::const_iterator &end) {
    if (!std::holds_alternative<std::string>(iter->second)) {
        throw JsonInvalidPairKeyException(iter->first);
    }

    std::string key = std::get<std::string>(iter->second);
    ++iter;

    if (iter == end) {
        --iter;
        throw JsonIncompleteObjectException(iter->first);
    }

    if (!std::holds_alternative<JsonSpecialToken>(iter->second) || std::get<JsonSpecialToken>(iter->second) != KEY_VALUE_SEAPARATOR) {
        throw JsonExpectedKeyValueSeparatorException(iter->first);
    }

    ++iter;

    if (iter == end) {
        --iter;
        throw JsonIncompleteObjectException(iter->first);
    }

    const JsonValue value = Json::parse_json_value(iter, end);

    data[key] = value;
}

// Function does not check OBJECT_BEGIN
void JsonObject::parse_json_object(std::map<int, JsonToken>::const_iterator &iter, const std::map<int, JsonToken>::const_iterator &end) {
    while (iter != end) {
        parse_json_object_pair(iter, end);

        if (iter != end) {
            // Should be followed by separator or end object
            if (!std::holds_alternative<JsonSpecialToken>(iter->second)) {
                throw JsonUnexpectedTokenException(iter->first);
            }

            auto tok = std::get<JsonSpecialToken>(iter->second);

            if (tok == SEPARATOR) {
                ++iter;
            } else if (tok == END_OBJECT) {
                ++iter;
                break;
            } else {
                throw JsonUnexpectedTokenException(iter->first);
            }
        }
    }
}
