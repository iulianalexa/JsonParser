#include "CommandLexer.h"

#include "CommandExceptions.h"
#include "utils.h"

bool is_escape_char(const char c) {
    return c == '\\';
}

bool is_name_char(const char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

CommandLexer::CommandLexer(const std::string &in) {
    this->in = in;
}

// Does not check COMMAND_ARRAY_SUBSCRIPT_BEGIN
std::map<long, CommandBit> CommandLexer::lex_command_in_index(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const {
    std::map<long, CommandBit> out_tokens = lex_command(iter, end, pos);

    if (iter == end || *iter != COMMAND_ARRAY_SUBSCRIPT_END) {
        throw CommandExpectedArraySubscriptCloseException(pos);
    }

    ++iter;
    pos++;
    return out_tokens;
}

std::map<long, CommandBit> CommandLexer::lex_command(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const {
    std::map<long, CommandBit> out_tokens;

    // All whitespaces are skippable here
    while (iter != end && *iter == COMMAND_WHITESPACE) {
        ++iter;
        pos++;
    }

    std::pair<long, CommandBit> bit = lex_bit(iter, end, pos);
    out_tokens.insert(bit);

    while (iter != end && *iter == COMMAND_OBJECT_SELECTOR) {
        ++iter;
        pos++;

        bit = lex_bit(iter, end, pos);
        out_tokens.insert(bit);
    }

    // All whitespaces are skippable here
    while (iter != end && *iter == COMMAND_WHITESPACE) {
        ++iter;
        pos++;
    }

    return out_tokens;
}

std::pair<long, CommandBit> CommandLexer::lex_bit(std::string::const_iterator &iter, const std::string::const_iterator &end, int &pos) const {
    // Get string, then indices

    long pair1 = pos;

    std::string str_token;
    int rc = lex_string(iter, end, str_token);
    if (rc == 0) {
        throw CommandExpectedStringSuperscriptException(pos);
    }

    pos += rc;

    // Get indices
    std::map<long, std::variant<std::map<long, CommandBit>, long>> indices;
    while (iter != end && *iter == COMMAND_ARRAY_SUBSCRIPT_BEGIN) {
        ++iter;
        pos++;

        long prev_pos = pos;

        // Might be an index (long)
        long num_token;
        rc = lex_long(iter, end, num_token);
        if (rc > 0) {
            if (iter == end || *iter != COMMAND_ARRAY_SUBSCRIPT_END) {
                throw CommandExpectedArraySubscriptCloseException(pos);
            }

            indices[pos] = num_token;
            pos += rc;
            pos++;
            ++iter;
        } else {
            std::map<long, CommandBit> subcommand = lex_command_in_index(iter, end, pos);
            indices[prev_pos] = subcommand;
        }
    }

    return std::make_pair(pair1, CommandBit(str_token, indices));
}

std::map<long, CommandBit> CommandLexer::lex() const {
    auto iter = in.cbegin();
    auto end = in.cend();
    int pos = 0;
    return lex_command(iter, end, pos);
}

int CommandLexer::lex_number(std::string::const_iterator &iter, const std::string::const_iterator &end, std::variant<long, long double> &out) {
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

int CommandLexer::lex_long(std::string::const_iterator &iter, const std::string::const_iterator &end, long &out) {
    if (*iter >= '0' && *iter <= '9') {
        auto end_num_iter = iter;
        for (; end_num_iter != end; ++end_num_iter) {
            if (*end_num_iter < '0' || *end_num_iter > '9') {
                break;
            }
        }

        out = std::stol(std::string(iter, end_num_iter));

        const int len = end_num_iter - iter;
        iter = end_num_iter;

        return len;
    }

    return 0;
}

int CommandLexer::lex_string(std::string::const_iterator &iter, const std::string::const_iterator &end, std::string &out) {
    int cnt = 0;
    std::string result;
    bool in_escape = false;

    while (is_escape_char(*iter) || is_name_char(*iter) || in_escape) {
        if (is_escape_char(*iter)) {
            if (!in_escape) {
                in_escape = true;
            } else {
                in_escape = false;
                result += *iter;
            }
        } else if (in_escape) {
            in_escape = false;
            result += *iter;
        } else {
            result += *iter;
        }

        cnt++;
        ++iter;
    }

    out = result;
    return cnt;
}
