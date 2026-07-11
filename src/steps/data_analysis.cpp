#include "data_analysis.h"
#include "../exceptions/max_capacity_exceeded.h"
#include <cctype>
#include <format>
#include <unordered_set>

using namespace qr;

const std::unordered_set<char> SPECIAL_CHARS{' ', '$', '%', '*', '+',
                                             '-', '.', '/', ':'};
EncodingMode analyze_encoding_mode(const std::string& input) {
    auto curr = input.begin();

    while (curr != input.end() && std::isdigit(*curr)) {
        curr++;
    }
    if (curr == input.end()) {
        return EncodingMode::NUMERIC;
    }

    while (curr != input.end() && (std::isdigit(*curr) || std::isupper(*curr) ||
                                   SPECIAL_CHARS.contains(*curr))) {
        curr++;
    }

    if (curr == input.end()) {
        return EncodingMode::ALPHANUMERIC;
    }

    return EncodingMode::BYTE;
}

unsigned char
determine_smallest_version(size_t input_len, EncodingMode mode,
                           ErrorCorrectionLevel correction_level) {
    int version = 1;

    while (version <= VERSIONS_AVAILABLE &&
           VERSION_CAPACITIES.at(correction_level).at(mode)[version - 1] <
               input_len) {
        version++;
    }

    if (version > VERSIONS_AVAILABLE) {
        throw MaxCapacityExceededException(
            std::format("Maximum capacity ({}) exceeded",
                        VERSION_CAPACITIES.at(correction_level)
                            .at(mode)[VERSIONS_AVAILABLE - 1]));
    }

    return version;
}
