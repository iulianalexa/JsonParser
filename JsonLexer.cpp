#include "JsonLexer.h"

#include <iostream>
#include <optional>

#include "JsonExceptions.h"

JsonLexer::JsonLexer(const std::string &in) {
    this->in = in;
}

int JsonLexer::lex_bool(std::string::const_iterator &iter, const std::string::const_iterator &end, JsonBool &out) {
    static std::string json_true = JSON_TRUE;
    static std::string json_false = JSON_FALSE;
    static std::string json_null = JSON_NULL;

    if (std::search(iter, end, json_true.begin(), json_true.end()) == iter) {
        out = JsonTrue;
        iter += static_cast<int>(json_true.size());
        return static_cast<int>(json_true.size());
    }

    if (std::search(iter, end, json_false.begin(), json_false.end()) == iter) {
        out = JsonFalse;
        iter += static_cast<int>(json_false.size());
        return static_cast<int>(json_false.size());
    }

    if (std::search(iter, end, json_null.begin(), json_null.end()) == iter) {
        out = JsonNull;
        iter += static_cast<int>(json_null.size());
        return static_cast<int>(json_null.size());
    }

    return 0;
}

int JsonLexer::lex_number(std::string::const_iterator &iter, const std::string::const_iterator &end, std::variant<long, long double> &out) {
    bool has_decimals = false;

    if ((*iter >= '0' && *iter <= '9') || *iter == '.') {
        if (*iter == '.') {
            has_decimals = true;
        }

        auto end_num_iter = iter;
        for (; end_num_iter != end; ++end_num_iter) {
            if (*end_num_iter == '.') {
                if (has_decimals) {
                    break;
                }

                has_decimals = true;
            } else if (*end_num_iter < '0' || *end_num_iter > '9') {
                break;
            }
        }

        if (*(end_num_iter - 1) == '.') {
            // if ends in decimal point
            --end_num_iter;
        }

        if (has_decimals) {
            out = std::stold(std::string(iter, end_num_iter));
        } else {
            out = std::stol(std::string(iter, end_num_iter));
        }

        const int len = end_num_iter - iter;
        iter = end_num_iter;

        return len;
    }

    return 0;
}

int JsonLexer::lex_string(std::string::const_iterator &iter, const std::string::const_iterator &end, std::string &out) {
    if (*iter == '"') {
        // Find ending quote
        auto end_quote_iter = std::find(iter + 1, end, '"');
        if (end_quote_iter == end) {
            throw JsonIncompleteStringException(iter - in.begin());
        }

        int len = end_quote_iter - iter + 1;
        out = std::string(iter + 1, end_quote_iter);
        iter = end_quote_iter + 1;
        return len;
    }

    return 0;
}

std::map<int, JsonToken> JsonLexer::lex() {
    int index = 0;
    std::map<int, JsonToken> out_tokens;
    auto iter = in.cbegin();
    auto end = in.cend();

    while (iter != end) {
        std::string json_str;
        int rc = lex_string(iter, end, json_str);

        if (rc > 0) {
            out_tokens[index] = json_str;
            index += rc;
            continue;
        }

        std::variant<long, long double> num;
        rc = lex_number(iter, end, num);

        if (rc > 0) {
            if (std::holds_alternative<long>(num)) {
                out_tokens[index] = std::get<long>(num);
            } else if (std::holds_alternative<long double>(num)) {
                out_tokens[index] = std::get<long double>(num);
            }

            index += rc;
            continue;
        }

        JsonBool json_bool;
        rc = lex_bool(iter, end, json_bool);

        if (rc > 0) {
            out_tokens[index] = json_bool;
            index += rc;
            continue;
        }

        if (in[index] == JSON_WHITESPACE) {
            index++;
            ++iter;
            continue;
        }

        if (in[index] == JSON_NEWLINE) {
            index++;
            ++iter;
            continue;
        }

        if (in[index] == JSON_BEGIN_OBJECT) {
            out_tokens[index] = BEGIN_OBJECT;
            index++;
            ++iter;
            continue;
        }

        if (in[index] == JSON_END_OBJECT) {
            out_tokens[index] = END_OBJECT;
            index++;
            ++iter;
            continue;
        }

        if (in[index] == JSON_SEPARATOR) {
            out_tokens[index] = SEPARATOR;
            index++;
            ++iter;
            continue;
        }

        if (in[index] == JSON_BEGIN_ARRAY) {
            out_tokens[index] = BEGIN_ARRAY;
            index++;
            ++iter;
            continue;
        }

        if (in[index] == JSON_END_ARRAY) {
            out_tokens[index] = END_ARRAY;
            index++;
            ++iter;
            continue;
        }

        if (in[index] == JSON_KEY_VALUE_SEPARATOR) {
            out_tokens[index] = KEY_VALUE_SEAPARATOR;
            index++;
            ++iter;
            continue;
        }

        throw JsonUnexpectedTokenException(index);
    }

    return out_tokens;
}
