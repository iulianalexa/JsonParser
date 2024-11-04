#ifndef COMMANDEXCEPTIONS_H
#define COMMANDEXCEPTIONS_H
#include <exception>
#include <string>

class CommandException : public std::exception {
protected:
    std::string message = "Exception";

    CommandException() = default;

    explicit CommandException(const char *err_msg);

    CommandException(int index, const char *err_msg);

    [[nodiscard]] const char *what() const noexcept override;
};

class CommandUnexpectedTokenException final : public CommandException {
public:
    static constexpr auto message = "Unexpected token.";

    CommandUnexpectedTokenException() : CommandException(message) {}
    explicit CommandUnexpectedTokenException(const int index) : CommandException(index, message) {}
};

class CommandExpectedArraySubscriptCloseException final : public CommandException {
public:
    static constexpr auto message = "Expected array subscript close.";

    CommandExpectedArraySubscriptCloseException() : CommandException(message) {}
    explicit CommandExpectedArraySubscriptCloseException(const int index) : CommandException(index, message) {}
};

class CommandExpectedStringSuperscriptException final : public CommandException {
public:
    static constexpr auto message = "Expected string type superscript.";

    CommandExpectedStringSuperscriptException() : CommandException(message) {}
    explicit CommandExpectedStringSuperscriptException(const int index) : CommandException(index, message) {}
};

class CommandExpectedObjectSelectorException final : public CommandException {
public:
    static constexpr auto message = "Expected object selector.";

    CommandExpectedObjectSelectorException() : CommandException(message) {}
    explicit CommandExpectedObjectSelectorException(const int index) : CommandException(index, message) {}
};

class CommandNotObjectException final : public CommandException {
public:
    static constexpr auto message = "Attempted object subscripting on a non-object.";

    CommandNotObjectException() : CommandException(message) {}
    explicit CommandNotObjectException(const int index) : CommandException(index, message) {}
};

class CommandNotArrayException final : public CommandException {
public:
    static constexpr auto message = "Attempted array subscripting on a non-array.";

    CommandNotArrayException() : CommandException(message) {}
    explicit CommandNotArrayException(const int index) : CommandException(index, message) {}
};

class CommandNotLongException final : public CommandException {
public:
    static constexpr auto message = "Attempted array subscripting with a non-long index.";

    CommandNotLongException() : CommandException(message) {}
    explicit CommandNotLongException(const int index) : CommandException(index, message) {}
};

class CommandArrayIndexOutOfRange final : public CommandException {
public:
    static constexpr auto message = "Attempted array subscripting with an out of range index.";

    CommandArrayIndexOutOfRange() : CommandException(message) {}
    explicit CommandArrayIndexOutOfRange(const int index) : CommandException(index, message) {}
};

class CommandKeyNotFoundException final : public CommandException {
public:
    static constexpr auto message = "Given key not found.";

    CommandKeyNotFoundException() : CommandException(message) {}
    explicit CommandKeyNotFoundException(const int index) : CommandException(index, message) {}
};

class CommandExpectedFunctionCloseException final : public CommandException {
public:
    static constexpr auto message = "Expected function close.";

    CommandExpectedFunctionCloseException() : CommandException(message) {}
    explicit CommandExpectedFunctionCloseException(const int index) : CommandException(index, message) {}
};

class CommandUnsupportedFunctionCallException final : public CommandException {
public:
    static constexpr auto message = "Unsupported function call for given argument types.";

    CommandUnsupportedFunctionCallException() : CommandException(message) {}
    explicit CommandUnsupportedFunctionCallException(const int index) : CommandException(index, message) {}
};

#endif //COMMANDEXCEPTIONS_H