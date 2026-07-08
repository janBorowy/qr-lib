#include "test_utils.h"

std::vector<bool> str_to_bits(const std::string& str) {
    std::vector<bool> result;
    for (const auto& c : str) {
        if (c == '1') {
            result.push_back(1);
        } else if (c == '0') {
            result.push_back(0);
        }
    }
    return result;
}
