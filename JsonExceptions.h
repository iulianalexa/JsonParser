#ifndef JSONEXCEPTIONS_H
#define JSONEXCEPTIONS_H

#include <exception>
#include <string>

class JsonException : public std::exception {
protected:
    std::string message = "Exception";

    JsonException() = default;

    explicit JsonException(const char *err_msg);

    JsonException(int index, const char *err_msg);

    [[nodiscard]] const char *what() const noexcept override;
};

class JsonIncompleteStringException final : public JsonException {
public:
    static constexpr auto message = "Incomplete string.";

    JsonIncompleteStringException() : JsonException(message) {}
    explicit JsonIncompleteStringException(const int index) : JsonException(index, message) {}
};

class JsonIncompleteObjectException final : public JsonException {
public:
    static constexpr auto message = "Incomplete object.";

    JsonIncompleteObjectException() : JsonException(message) {}
    explicit JsonIncompleteObjectException(const int index) : JsonException(index, message) {}
};

class JsonInvalidPairKeyException final : public JsonException {
public:
    static constexpr auto message = "Invalid pair key (must be string).";

    JsonInvalidPairKeyException() : JsonException(message) {}
    explicit JsonInvalidPairKeyException(const int index) : JsonException(index, message) {}

};

class JsonExpectedKeyValueSeparatorException final : public JsonException {
public:
    static constexpr auto message = "Expected key/value separator.";

    JsonExpectedKeyValueSeparatorException() : JsonException(message) {}
    explicit JsonExpectedKeyValueSeparatorException(const int index) : JsonException(index, message) {}
};

class JsonUnexpectedTokenException final : public JsonException {
public:
    static constexpr auto message = "Unexpected token.";

    JsonUnexpectedTokenException() : JsonException(message) {}
    explicit JsonUnexpectedTokenException(const int index) : JsonException(index, message) {}
};

class JsonEmptyStringException final : public JsonException {
public:
    static constexpr auto message = "Received empty string.";

    JsonEmptyStringException() : JsonException(message) {}
    explicit JsonEmptyStringException(const int index) : JsonException(index, message) {}
};

#endif //JSONEXCEPTIONS_H
