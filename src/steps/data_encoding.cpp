#include "data_encoding.h"
#include "data_analysis.h"
#include <bitset>

std::string get_mode_indicator(EncodingMode mode) {
    switch (mode) {
    case EncodingMode::NUMERIC:
        return "0001";
    case EncodingMode::ALPHANUMERIC:
        return "0010";
    case EncodingMode::BYTE:
        return "0100";
    }
}

int get_character_count_length(EncodingMode mode, unsigned char version) {
    switch (mode) {
    case EncodingMode::NUMERIC:
        if (version < 10) {
            return 10;
        } else if (version < 27) {
            return 12;
        }
        return 14;
    case EncodingMode::ALPHANUMERIC:
        if (version < 10) {
            return 9;
        } else if (version < 27) {
            return 11;
        }
        return 13;
    case EncodingMode::BYTE:
        if (version < 10) {
            return 8;
        }
        return 16;
    }
}

std::string encoded_message(const std::string& message, EncodingMode mode) {
    std::string encoded_message;
}

std::string encode_data(const std::string& data, EncodingMode mode,
                        unsigned char version) {
    auto mode_indicator = get_mode_indicator(mode);
    auto count_indicator_len = get_character_count_length(mode, version);
    auto count_indicator_str = std::bitset<32>(data.size()).to_string();
    count_indicator_str = count_indicator_str.substr(
        count_indicator_str.size() - count_indicator_len);
    auto encoded_message = encode_message(data, mode);
    return mode_indicator + count_indicator_str;
}
