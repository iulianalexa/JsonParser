#include "CommandExceptions.h"

CommandException::CommandException(const char *err_msg) {
    message = std::string("Exception parsing command: ") + std::string(err_msg) + std::string("\n");
}

CommandException::CommandException(const int index, const char *err_msg) {
    message = std::string("Exception parsing command at position ") + std::to_string(index) + std::string(": ") + std::string(err_msg) + std::string("\n");
}

const char *CommandException::what() const noexcept {
    return message.c_str();
}
