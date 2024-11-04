#ifndef COMMANDLEXER_H
#define COMMANDLEXER_H
#include <map>
#include <string>

#include "utils.h"
#include "CommandBit.h"
#include "CommandFunc.h"

bool is_escape_char(char c);

bool is_name_char(char c);

class CommandLexer {
protected:
    explicit CommandLexer(const std::string &in);

    [[nodiscard]] CommandFunc lex() const;

private:
    friend class CommandBit;

    static CommandFuncType check_func(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos);

    CommandFunc lex_command_func(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const;

    CommandFunc lex_command_in_index(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const;

    Command lex_command(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const;

    Command::value_type lex_bit(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const;

    static int lex_long(std::string::const_iterator &iter, const std::string::const_iterator &end, long &out);

    static int lex_number(std::string::const_iterator &iter, const std::string::const_iterator &end, std::variant<long, long double> &out);

    static int lex_string(std::string::const_iterator &iter, const std::string::const_iterator &end, std::string &out);

    std::string in;
};

#endif //COMMANDLEXER_H
