#include "JsonExceptions.h"

JsonException::JsonException(const char *err_msg) {
    message = std::string("Exception parsing JSON: ") + std::string(err_msg) + std::string("\n");
}

JsonException::JsonException(const int index, const char *err_msg) {
    message = std::string("Exception parsing JSON at position ") + std::to_string(index) + std::string(": ") + std::string(err_msg) + std::string("\n");
}

const char *JsonException::what() const noexcept {
    return message.c_str();
}
