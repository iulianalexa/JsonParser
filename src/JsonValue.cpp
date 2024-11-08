#include "JsonValue.h"

#include "JsonObject.h"
#include "JsonArray.h"

JsonValue::JsonValue(const std::variant<std::string, long, long double, JsonBool, Box<JsonObject>, Box<JsonArray>> &data) : data(data) {}

std::variant<std::string, long, long double, JsonBool, JsonObject, JsonArray> JsonValue::get() const {
    if (std::holds_alternative<Box<JsonObject>>(data)) {
        return std::get<Box<JsonObject>>(data);
    }

    if (std::holds_alternative<Box<JsonArray>>(data)) {
        return std::get<Box<JsonArray>>(data);
    }

    if (std::holds_alternative<long>(data)) {
        return std::get<long>(data);
    }

    if (std::holds_alternative<long double>(data)) {
        return std::get<long double>(data);
    }

    if (std::holds_alternative<JsonBool>(data)) {
        return std::get<JsonBool>(data);
    }

    return std::get<std::string>(data);
}

long double JsonValue::min() const {
    auto alternative = get();
    if (std::holds_alternative<long>(alternative)) {
        return std::get<long>(alternative);
    }

    if (std::holds_alternative<long double>(alternative)) {
        return std::get<long double>(alternative);
    }

    if (std::holds_alternative<JsonArray>(alternative)) {
        auto arr = std::get<JsonArray>(alternative).get();
        if (arr.empty()) {
            throw std::invalid_argument("");
        }

        long double minimum = arr[0].min();
        for (auto it = arr.begin() + 1; it != arr.end(); ++it) {
            const long double val = it->min();
            if (val < minimum) {
                minimum = val;
            }
        }

        return minimum;
    }

    throw std::invalid_argument("");
}

long double JsonValue::max() const {
    auto alternative = get();
    if (std::holds_alternative<long>(alternative)) {
        return std::get<long>(alternative);
    }

    if (std::holds_alternative<long double>(alternative)) {
        return std::get<long double>(alternative);
    }

    if (std::holds_alternative<JsonArray>(alternative)) {
        auto arr = std::get<JsonArray>(alternative).get();
        if (arr.empty()) {
            throw std::invalid_argument("");
        }

        long double maximum = arr[0].max();
        for (auto it = arr.begin() + 1; it != arr.end(); ++it) {
            const long double val = it->max();
            if (val > maximum) {
                maximum = val;
            }
        }

        return maximum;
    }

    throw std::invalid_argument("");
}

unsigned long JsonValue::size() const {
    auto alternative = get();

    if (std::holds_alternative<JsonArray>(alternative)) {
        return std::get<JsonArray>(alternative).get().size();
    }

    if (std::holds_alternative<JsonObject>(alternative)) {
        return std::get<JsonObject>(alternative).get().size();
    }

    if (std::holds_alternative<std::string>(alternative)) {
        return std::get<std::string>(alternative).size();
    }

    throw std::invalid_argument("");
}

std::ostream &operator<<(std::ostream &os, const JsonValue &json_value) {
    auto alternative = json_value.get();

    if (std::holds_alternative<std::string>(alternative)) {
        os << '"' << std::get<std::string>(alternative) << '"';
    } else if (std::holds_alternative<long>(alternative)) {
        os << std::get<long>(alternative);
    } else if (std::holds_alternative<long double>(alternative)) {
        os << std::get<long double>(alternative);
    } else if (std::holds_alternative<JsonBool>(alternative)) {
        auto json_bool = std::get<JsonBool>(alternative);
        if (json_bool == JsonTrue) {
            os << "true";
        } else if (json_bool == JsonFalse) {
            os << "false";
        } else if (json_bool == JsonNull) {
            os << "null";
        }
    } else if (std::holds_alternative<JsonObject>(alternative)) {
        os << std::get<JsonObject>(alternative);
    } else if (std::holds_alternative<JsonArray>(alternative)) {
        os << std::get<JsonArray>(alternative);
    }

    return os;
}