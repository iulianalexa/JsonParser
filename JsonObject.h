#ifndef JSONOBJECT_H
#define JSONOBJECT_H
#include <map>

#include "JsonLexer.h"
#include "JsonValue.h"

class JsonObject : public JsonLexer {
public:
    explicit JsonObject(const std::string &in);

    [[nodiscard]] std::map<std::string, JsonValue> get() const;

private:
    friend class JsonArray;
    friend class Json;

    JsonObject() = default;

    std::map<std::string, JsonValue> data;

    void parse();
    void parse_json_object_pair(std::map<int, JsonToken>::const_iterator &iter, const std::map<int, JsonToken>::const_iterator &end);
    void parse_json_object(std::map<int, JsonToken>::const_iterator &iter, const std::map<int, JsonToken>::const_iterator &end);
};

std::ostream &operator<<(std::ostream &os, const JsonObject &json_object);

#endif //JSONOBJECT_H
