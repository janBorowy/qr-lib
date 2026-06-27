#include "data_analysis.h"
#include "../exceptions/max_capacity_exceeded.h"
#include "parameters.h"
#include <cctype>
#include <format>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const std::unordered_set<char> SPECIAL_CHARS{' ', '$', '%', '*', '+',
                                             '-', '.', '/', ':'};
// TODO: fill this, L and M only for now and up to 10 verions
constexpr int VERSIONS_AVAILBALE = 10;
const std::unordered_map<ErrorCorrectionLevel,
                         std::unordered_map<EncodingMode, std::vector<int>>>
    VERSION_CAPACITIES = {
        {ErrorCorrectionLevel::L,
         {{EncodingMode::NUMERIC,
           std::vector<int>{41, 77, 127, 187, 255, 322, 370, 461, 552, 652}},
          {EncodingMode::ALPHANUMERIC,
           std::vector<int>{25, 47, 77, 114, 154, 195, 224, 279, 335, 395}},
          {EncodingMode::BYTE,
           std::vector<int>{17, 32, 53, 78, 106, 134, 154, 192, 230, 271}}}},
        {ErrorCorrectionLevel::M,
         {{EncodingMode::NUMERIC,
           std::vector<int>{34, 63, 101, 149, 202, 255, 293, 365, 432, 514}},
          {EncodingMode::ALPHANUMERIC,
           std::vector<int>{20, 38, 61, 90, 122, 154, 178, 221, 262, 311}},
          {EncodingMode::BYTE,
           std::vector<int>{14, 26, 42, 62, 84, 106, 122, 152, 180, 213}}}}};

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

    while (version <= VERSIONS_AVAILBALE &&
           VERSION_CAPACITIES.at(correction_level).at(mode)[version - 1] <
               input_len) {
        version++;
    }

    if (version > VERSIONS_AVAILBALE) {
        throw MaxCapacityExceededException(
            std::format("Maximum capacity ({}) exceeded",
                        VERSION_CAPACITIES.at(correction_level)
                            .at(mode)[VERSIONS_AVAILBALE - 1]));
    }

    return version;
}
