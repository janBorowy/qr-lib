#include "data_encoding.h"
#include "data_analysis.h"
#include "parameters.h"
#include <bitset>
#include <iterator>
#include <unordered_map>
#include <vector>

std::unordered_map<char, int> ALPHANUMERIC_TO_VAL = {
    {'0', 0},  {'1', 1},  {'2', 2},  {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},
    {'7', 7},  {'8', 8},  {'9', 9},  {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13},
    {'E', 14}, {'F', 15}, {'G', 16}, {'H', 17}, {'I', 18}, {'J', 19}, {'K', 20},
    {'L', 21}, {'M', 22}, {'N', 23}, {'O', 24}, {'P', 25}, {'Q', 26}, {'R', 27},
    {'S', 28}, {'T', 29}, {'U', 30}, {'V', 31}, {'W', 32}, {'X', 33}, {'Y', 34},
    {'Z', 35}, {' ', 36}, {'$', 37}, {'%', 38}, {'*', 39}, {'+', 40}, {'-', 41},
    {'.', 42}, {'/', 43}, {':', 44}};

void encode_mode_indicator(EncodingMode mode, std::vector<bool>& result) {
    switch (mode) {
    case EncodingMode::NUMERIC:
        result.insert(result.end(), {0, 0, 0, 1});
        break;
    case EncodingMode::ALPHANUMERIC:
        result.insert(result.end(), {0, 0, 1, 0});
        break;
    case EncodingMode::BYTE:
        result.insert(result.end(), {0, 1, 0, 0});
        break;
    }
}

template <unsigned int N>
void push_back_bits(int len, const std::bitset<N>& bits,
                    std::vector<bool>& result) {
    for (int i = len - 1; i >= 0; i--) {
        result.push_back(bits[i]);
    }
}

void encode_count_indicator(EncodingMode mode, unsigned char version,
                            size_t data_len, std::vector<bool>& result) {
    int count_indicator_len = 0;
    switch (mode) {
    case EncodingMode::NUMERIC:
        if (version < 10) {
            count_indicator_len = 10;
        } else if (version < 27) {
            count_indicator_len = 12;
        } else {
            count_indicator_len = 14;
        }
        break;
    case EncodingMode::ALPHANUMERIC:
        if (version < 10) {
            count_indicator_len = 9;
        } else if (version < 27) {
            count_indicator_len = 11;
        } else {
            count_indicator_len = 13;
        }
        break;
    case EncodingMode::BYTE:
        if (version < 10) {
            count_indicator_len = 8;
        } else {
            count_indicator_len = 16;
        }
        break;
    }

    push_back_bits<32>(count_indicator_len, std::bitset<32>(data_len), result);
}

void encode_numeric_group(const std::string& numeric_str,
                          std::vector<bool>& result) {
    auto grouped_bits = std::bitset<32>(std::stoi(numeric_str));

    int encoded_size = 10;
    if (numeric_str.size() == 2) {
        encoded_size = 7;
    } else if (numeric_str.size() == 1) {
        encoded_size = 4;
    }

    push_back_bits<32>(encoded_size, grouped_bits, result);
}

void encode_numeric(const std::string& message, std::vector<bool>& result) {
    int k = 0;

    for (auto i = 0; i != message.size(); i++) {
        k++;
        if (k == 3) {
            encode_numeric_group(message.substr(i - 2, 3), result);
            k = 0;
        }
    }

    if (k == 2) {
        encode_numeric_group(message.substr(message.size() - 2, 2), result);
    } else if (k == 1) {
        encode_numeric_group(message.substr(message.size() - 1, 1), result);
    }
}

void encode_alpha(const std::string& message, std::vector<bool>& result) {
    bool is_even = false;
    char prev;
    for (const auto& c : message) {
        if (is_even) {
            int value = ALPHANUMERIC_TO_VAL[prev] * 45 + ALPHANUMERIC_TO_VAL[c];
            push_back_bits<32>(11, std::bitset<32>(value), result);
        }
        prev = c;
        is_even = !is_even;
    }

    if (is_even) {
        auto bits =
            std::bitset<32>(ALPHANUMERIC_TO_VAL[message[message.length() - 1]]);
        push_back_bits<32>(6, bits, result);
    }
}
void encode_byte(const std::string& message, std::vector<bool>& result) {
    for (const auto& c : message) {
        push_back_bits<8>(8, std::bitset<8>(c), result);
    }
}

void encode_message(const std::string& message, EncodingMode mode,
                    std::vector<bool>& result) {
    std::string encoded_message;
    switch (mode) {
    case EncodingMode::NUMERIC:
        encode_numeric(message, result);
        break;
    case EncodingMode::ALPHANUMERIC:
        encode_alpha(message, result);
        break;
    case EncodingMode::BYTE:
        encode_byte(message, result);
        break;
    }
}

std::vector<bool> encode_data(const std::string& data, EncodingMode mode,
                              unsigned char version, ErrorCorrectionLevel ec_level) {
    std::vector<bool> result;
    encode_mode_indicator(mode, result);
    encode_count_indicator(mode, version, data.size(), result);
    encode_message(data, mode, result);
    return result;
}
