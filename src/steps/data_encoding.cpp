#include "data_encoding.h"
#include "data_analysis.h"
#include <bitset>
#include <unordered_map>
#include <vector>

typedef std::bitset<sizeof(int) * 8> int_bits;

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

    auto count_indicator_bits = int_bits(data_len);

    for (int i = count_indicator_len - 1; i >= 0; i--) {
        result.push_back(count_indicator_bits[i]);
    }
}

void encode_numeric_group(const std::string& numeric_str,
                          std::vector<bool>& output) {
    auto grouped_bits = int_bits(std::stoi(numeric_str));

    int encoded_size = 10;
    if (numeric_str.size() == 2) {
        encoded_size = 7;
    } else if (numeric_str.size() == 1) {
        encoded_size = 4;
    }

    for (int i = encoded_size - 1; i >= 0; i--) {
        output.push_back(grouped_bits[i]);
    }
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

void encode_alpha(const std::string& message, std::vector<bool>& result) {}
void encode_byte(const std::string& message, std::vector<bool>& result) {}

void encode_message(const std::string& message, EncodingMode mode,
                    std::vector<bool>& result) {
    std::string encoded_message;
    switch (mode) {
    case EncodingMode::NUMERIC:
        encode_numeric(message, result);
    case EncodingMode::ALPHANUMERIC:
        encode_alpha(message, result);
    case EncodingMode::BYTE:
        encode_byte(message, result);
    }
}

std::vector<bool> encode_data(const std::string& data, EncodingMode mode,
                              unsigned char version) {
    std::vector<bool> result;
    encode_mode_indicator(mode, result);
    encode_count_indicator(mode, version, data.size(), result);
    encode_message(data, mode, result);
    return result;
}
