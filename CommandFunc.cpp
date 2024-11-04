#include "CommandFunc.h"

CommandFunc::CommandFunc(const std::map<long, std::variant<CommandFunc, Command, long, long double>> &args, const CommandFuncType &func_type) : args(args), func_type(func_type) {}
