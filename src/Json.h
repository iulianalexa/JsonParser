#ifndef JSON_H
#define JSON_H
#include <variant>
#include <map>

#include "JsonObject.h"
#include "JsonArray.h"
#include "JsonValue.h"

class Json {
    friend class JsonArray;
    friend class JsonObject;

    static JsonValue parse_json_value(std::map<int, JsonToken>::const_iterator &iter, const std::map<int, JsonToken>::const_iterator &end);
};

#endif //JSON_H
