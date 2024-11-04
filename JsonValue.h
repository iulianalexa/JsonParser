#ifndef JSONVALUE_H
#define JSONVALUE_H
#include <variant>
#include <string>
#include "utils.h"
#include "Box.h"

class JsonObject;
class JsonArray;

class JsonValue {
public:
    explicit JsonValue(const std::variant<std::string, long, long double, JsonBool, Box<JsonObject>, Box<JsonArray>> &data);
    JsonValue() = default;

    [[nodiscard]] std::variant<std::string, long, long double, JsonBool, JsonObject, JsonArray> get() const;

    [[nodiscard]] unsigned long size() const;

    [[nodiscard]] long double min() const;

    [[nodiscard]] long double max() const;

private:
    std::variant<std::string, long, long double, JsonBool, Box<JsonObject>, Box<JsonArray>> data;
};

std::ostream &operator<<(std::ostream &os, const JsonValue &json_value);

#endif //JSONVALUE_H
