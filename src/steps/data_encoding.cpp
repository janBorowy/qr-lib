#include "data_encoding.h"
#include "qr.h"
#include <algorithm>
#include <bitset>
#include <vector>

using namespace qr;

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
            int value = qr::ALPHANUMERIC_TO_VAL.at(prev) * 45 +
                        qr::ALPHANUMERIC_TO_VAL.at(c);
            push_back_bits<32>(11, std::bitset<32>(value), result);
        }
        prev = c;
        is_even = !is_even;
    }

    if (is_even) {
        auto bits = std::bitset<32>(
            qr::ALPHANUMERIC_TO_VAL.at(message[message.length() - 1]));
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

void encode_terminator_and_padding(const std::string& data,
                                   unsigned char version,
                                   ErrorCorrectionLevel ec_level,
                                   std::vector<bool>& result) {
    auto data_bits = VERSION_AND_EC_TO_DATA_CODEWORDS[version][ec_level] * 8;
    auto terminator_len = std::min(data_bits - result.size(), 4uz);
    push_back_bits<4>(terminator_len, std::bitset<4>{"0000"}, result);
    auto padding_zeroes = (8 - result.size() % 8) % 8;
    push_back_bits<7>(padding_zeroes, std::bitset<7>{"0000000"}, result);
    auto is_even = false;
    while (result.size() < data_bits) {
        if (is_even) {
            push_back_bits<8>(8, std::bitset<8>{"11101100"}, result);
        } else {
            push_back_bits<8>(8, std::bitset<8>{"00010001"}, result);
        }
        is_even = !is_even;
    }
}

std::vector<bool> encode_data(const std::string& data, EncodingMode mode,
                              unsigned char version,
                              ErrorCorrectionLevel ec_level) {
    std::vector<bool> result;
    encode_mode_indicator(mode, result);
    encode_count_indicator(mode, version, data.size(), result);
    encode_message(data, mode, result);
    encode_terminator_and_padding(data, version, ec_level, result);
    return result;
}
