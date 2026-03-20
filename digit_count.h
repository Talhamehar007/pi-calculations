#ifndef DIGIT_COUNT_H
#define DIGIT_COUNT_H

#include <cctype>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string_view>

inline std::int64_t parse_digit_count(std::string_view text) {
    std::size_t begin = 0;
    std::size_t end = text.size();

    while (begin < end && std::isspace(static_cast<unsigned char>(text[begin]))) {
        ++begin;
    }
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }

    if (begin == end) {
        throw std::invalid_argument("digits must not be empty");
    }

    if (text[begin] == '+') {
        ++begin;
    } else if (text[begin] == '-') {
        throw std::invalid_argument("digits must be >= 1");
    }

    if (begin == end) {
        throw std::invalid_argument("digits must not be empty");
    }

    std::int64_t multiplier = 1;
    char suffix = text[end - 1];
    if (std::isalpha(static_cast<unsigned char>(suffix))) {
        switch (static_cast<char>(std::tolower(static_cast<unsigned char>(suffix)))) {
            case 'k':
                multiplier = 1000LL;
                break;
            case 'm':
                multiplier = 1000000LL;
                break;
            case 'b':
                multiplier = 1000000000LL;
                break;
            default:
                throw std::invalid_argument("unsupported digit suffix; use k, m, or b");
        }
        --end;
    }

    if (begin == end) {
        throw std::invalid_argument("digits must not be empty");
    }

    const std::int64_t max_base = std::numeric_limits<std::int64_t>::max() / multiplier;
    std::int64_t value = 0;
    bool saw_digit = false;

    for (std::size_t index = begin; index < end; ++index) {
        unsigned char ch = static_cast<unsigned char>(text[index]);
        if (std::isdigit(ch)) {
            saw_digit = true;
            int digit = ch - '0';
            if (value > (max_base - digit) / 10) {
                throw std::out_of_range("digit count is too large");
            }
            value = value * 10 + digit;
            continue;
        }

        if (ch == ',' || ch == '_') {
            continue;
        }

        throw std::invalid_argument("invalid digit count format");
    }

    if (!saw_digit) {
        throw std::invalid_argument("digits must contain at least one digit");
    }

    value *= multiplier;
    if (value < 1) {
        throw std::invalid_argument("digits must be >= 1");
    }

    return value;
}

#endif
