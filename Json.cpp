#include "Json.h"

#include <map>

#include "JsonArray.h"
#include "JsonExceptions.h"
#include "JsonObject.h"
#include "JsonValue.h"

JsonValue Json::parse_json_value(std::map<int, JsonToken>::const_iterator &iter, const std::map<int, JsonToken>::const_iterator &end) {
    JsonValue out;

    if (std::holds_alternative<JsonSpecialToken>(iter->second)) {
        JsonSpecialToken tok = std::get<JsonSpecialToken>(iter->second);
        if (tok == BEGIN_OBJECT) {
            ++iter;
            JsonObject obj;
            obj.parse_json_object(iter, end);
            out = JsonValue(obj);
        } else if (tok == BEGIN_ARRAY) {
            ++iter;
            JsonArray arr;
            arr.parse_json_array(iter, end);
            out = JsonValue(arr);
        } else {
            throw JsonUnexpectedTokenException(iter->first);
        }
    } else if (std::holds_alternative<std::string>(iter->second)) {
        out = JsonValue(std::get<std::string>(iter->second));
        ++iter;
    } else if (std::holds_alternative<long>(iter->second)) {
        out = JsonValue(std::get<long>(iter->second));
        ++iter;
    } else if (std::holds_alternative<long double>(iter->second)) {
        out = JsonValue(std::get<long double>(iter->second));
        ++iter;
    } else if (std::holds_alternative<JsonBool>(iter->second)) {
        out = JsonValue(std::get<JsonBool>(iter->second));
        ++iter;
    } else {
        throw JsonUnexpectedTokenException(iter->first);
    }

    return out;
}
