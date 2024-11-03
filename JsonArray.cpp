#include "JsonArray.h"

#include "JsonExceptions.h"

std::ostream &operator<<(std::ostream &os, const JsonArray &json_array) {
    const auto data = json_array.get();
    os << "[";
    const int size = data.size();

    for (int i = 0; i < size; i++) {
        os << data[i];

        if (i < size - 1) {
            os << ", ";
        }
    }

    os << "]";

    return os;
}

JsonArray::JsonArray(const std::string &in) : JsonLexer(in) {
    parse();
}

std::vector<JsonValue> JsonArray::get() const {
    return data;
}

void JsonArray::parse() {

    std::map<int, JsonToken> tokens = lex();

    if (tokens.empty()) {
        throw JsonEmptyStringException();
    }

    if (!std::holds_alternative<JsonSpecialToken>(tokens[0])) {
        throw JsonUnexpectedTokenException(0);
    }

    auto tok = std::get<JsonSpecialToken>(tokens[0]);

    if (tok != BEGIN_ARRAY) {
        throw JsonUnexpectedTokenException(0);
    }

    auto iter = tokens.cbegin();
    auto end = tokens.cend();
    ++iter;
    parse_json_array(iter, end);
    if (iter != end) {
        throw JsonUnexpectedTokenException(iter->first);
    }
}

// Function does not check ARRAY_BEGIN.
void JsonArray::parse_json_array(std::map<int, JsonToken>::const_iterator &iter, const std::map<int, JsonToken>::const_iterator &end) {
    while (iter != end) {
        JsonValue value = Json::parse_json_value(iter, end);

        data.emplace_back(value);

        if (iter != end) {
            // Should be followed by separator or end array
            if (!std::holds_alternative<JsonSpecialToken>(iter->second)) {
                throw JsonUnexpectedTokenException(iter->first);
            }

            auto tok = std::get<JsonSpecialToken>(iter->second);

            if (tok == SEPARATOR) {
                ++iter;
            } else if (tok == END_ARRAY) {
                ++iter;
                break;
            } else {
                throw JsonUnexpectedTokenException(iter->first);
            }
        }
    }
}
