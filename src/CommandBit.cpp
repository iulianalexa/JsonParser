#include "CommandBit.h"

#include <utility>

#include "CommandFunc.h"

CommandBit::CommandBit(std::string name, const std::map<long, std::variant<Box<CommandFunc>, long>> &array_indices) : array_indices(array_indices), name(std::move(name)) {}
