#include "test_utils.h"
#include <bitset>

std::vector<qr::Codeword> str_to_codewords(const std::string& str) {
    std::string clean;
    std::vector<qr::Codeword> result;
    for (const auto& c : str) {
        if (c == '1' || c == '0') {
            clean.push_back(c);
        }
    }
    int bytes = clean.size() / 8;
    for (int b = 0; b < bytes; b++) {
        auto bits = std::bitset<8>(clean.substr(8 * b, 8 * (b + 1)));
        result.push_back(bits.to_ulong());
    }
    return result;
}
