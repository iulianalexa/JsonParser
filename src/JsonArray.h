#ifndef JSONARRAY_H
#define JSONARRAY_H
#include <string>
#include <map>

#include "JsonLexer.h"
#include "Json.h"

std::ostream &operator<<(std::ostream &os, const JsonArray &json_array);

class JsonArray : public JsonLexer {
public:
    explicit JsonArray(const std::string &in);

    [[nodiscard]] std::vector<JsonValue> get() const;

private:
    friend class JsonObject;
    friend class Json;

    JsonArray() = default;

    std::vector<JsonValue> data;

    void parse();
    void parse_json_array(std::map<int, JsonToken>::const_iterator &iter, const std::map<int, JsonToken>::const_iterator &end);
};

#endif //JSONARRAY_H
