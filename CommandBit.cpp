#include "CommandBit.h"

#include <utility>

CommandBit::CommandBit(std::string name, const std::map<long, std::variant<std::map<long, CommandBit>, long>> &array_indices) : array_indices(array_indices), name(std::move(name)) {}
