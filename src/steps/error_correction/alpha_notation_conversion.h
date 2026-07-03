#pragma once

#include <array>

constexpr std::array<unsigned char, 256> make_alpha_to_int() {
    std::array<unsigned char, 256> result;
    result[0] = 1;
    for (int i = 1; i < 256; i++) {
        auto next_val = static_cast<int>(result[i - 1]) * 2;
        if (next_val > 255) {
            result[i] = next_val ^ 285;
        } else {
            result[i] = next_val;
        }
    }
    return result;
}

constexpr std::array<unsigned char, 256> ALPHA_TO_INT = make_alpha_to_int();

constexpr std::array<unsigned char, 256> make_int_to_alpha() {
    std::array<unsigned char, 256> result;
    for (int i = 1; i < 256; i++) {
        result[ALPHA_TO_INT[i]] = i;
    }
    return result;
}

constexpr std::array<unsigned char, 256> INT_TO_ALPHA = make_int_to_alpha();
