#ifndef JSONLEXER_H
#define JSONLEXER_H
#include <map>
#include <string>

#include "utils.h"

class JsonLexer {
protected:
    std::string in;

    JsonLexer() = default;
    explicit JsonLexer(const std::string &in);

    int lex_bool(std::string::const_iterator &iter, const std::string::const_iterator &end, JsonBool &out);
    int lex_number(std::string::const_iterator &iter, const std::string::const_iterator &end, std::variant<long, long double> &out);
    int lex_string(std::string::const_iterator &iter, const std::string::const_iterator &end, std::string &out);
    std::map<int, JsonToken> lex();
};

#endif //JSONLEXER_H
