#ifndef COMMANDLEXER_H
#define COMMANDLEXER_H
#include <map>
#include <string>

#include "CommandBit.h"

bool is_escape_char(char c);

bool is_name_char(char c);

class CommandLexer {
protected:
    explicit CommandLexer(const std::string &in);

    [[nodiscard]] std::map<long, CommandBit> lex() const;

private:
    friend class CommandBit;

    std::map<long, CommandBit> lex_command_in_index(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const;

    std::map<long, CommandBit> lex_command(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const;

    std::pair<long, CommandBit> lex_bit(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const;

    static int lex_long(std::string::const_iterator &iter, const std::string::const_iterator &end, long &out);

    static int lex_number(std::string::const_iterator &iter, const std::string::const_iterator &end, std::variant<long, long double> &out);

    static int lex_string(std::string::const_iterator &iter, const std::string::const_iterator &end, std::string &out);

    std::string in;
};

#endif //COMMANDLEXER_H
