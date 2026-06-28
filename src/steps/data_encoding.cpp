#include "data_encoding.h"
#include "data_analysis.h"
#include <bitset>
#include <vector>

typedef std::bitset<sizeof(int) * 8> int_bits;

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

std::vector<bool> encode_alpha(const std::string& message,
                               std::vector<bool>& result) {
    return std::vector<bool>{};
}
std::vector<bool> encode_byte(const std::string& message,
                              std::vector<bool>& result) {
    return std::vector<bool>{};
}

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
