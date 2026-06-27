#include "data_analysis.h"
#include <cctype>
#include <unordered_set>

const std::unordered_set<char> SPECIAL_CHARS{' ', '$', '%', '*', '+',
                                             '-', '.', '/', ':'};

ENCODING_MODE analyze_encoding_mode(std::string& input) {
    auto curr = input.begin();

    while (std::isdigit(*curr)) {
        curr++;
    }
    if (curr == input.end()) {
        return ENCODING_MODE::NUMERIC;
    }

    while (std::isdigit(*curr) || std::isupper(*curr) ||
           SPECIAL_CHARS.contains(*curr)) {
        curr++;
    }

    if (curr == input.end()) {
        return ENCODING_MODE::ALPHANUMERIC;
    }

    return ENCODING_MODE::BYTE;
}
