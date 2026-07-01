#include <bitset>
#include <catch2/catch_test_macros.hpp>

#include "../src/exceptions/max_capacity_exceeded.h"
#include "../src/steps/data_analysis.h"
#include "../src/steps/data_encoding.h"

#include <cstdint>

uint32_t factorial(uint32_t number) {
    return number <= 1 ? number : factorial(number - 1) * number;
}

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

std::string hex_to_bytes(std::vector<unsigned char> hex) {
    std::string result;
    for (const auto& c : hex) {
        result.append(std::bitset<8>(c).to_string());
    }

    return result;
}

TEST_CASE("Determines the most efficient encoding_mode", "[data_analysis]") {
    REQUIRE(analyze_encoding_mode("0123456789") == EncodingMode::NUMERIC);
    REQUIRE(analyze_encoding_mode("ABC") == EncodingMode::ALPHANUMERIC);
    REQUIRE(analyze_encoding_mode("abc") == EncodingMode::BYTE);
    REQUIRE(analyze_encoding_mode("012345678A") == EncodingMode::ALPHANUMERIC);
    REQUIRE(analyze_encoding_mode("A123456789") == EncodingMode::ALPHANUMERIC);
    REQUIRE(analyze_encoding_mode("a123456789") == EncodingMode::BYTE);
    REQUIRE(analyze_encoding_mode("012345678Aa") == EncodingMode::BYTE);
    REQUIRE(analyze_encoding_mode("a123456789") == EncodingMode::BYTE);
}

TEST_CASE("Determines the smallest sufficient version", "[data_analysis]") {
    REQUIRE(determine_smallest_version(11, EncodingMode::ALPHANUMERIC,
                                       ErrorCorrectionLevel::L) == 1);
    REQUIRE(determine_smallest_version(25, EncodingMode::ALPHANUMERIC,
                                       ErrorCorrectionLevel::L) == 1);
    REQUIRE(determine_smallest_version(26, EncodingMode::ALPHANUMERIC,
                                       ErrorCorrectionLevel::L) == 2);
    REQUIRE(determine_smallest_version(41, EncodingMode::NUMERIC,
                                       ErrorCorrectionLevel::L) == 1);
    REQUIRE(determine_smallest_version(42, EncodingMode::NUMERIC,
                                       ErrorCorrectionLevel::L) == 2);
    REQUIRE(determine_smallest_version(77, EncodingMode::NUMERIC,
                                       ErrorCorrectionLevel::L) == 2);

    REQUIRE(determine_smallest_version(34, EncodingMode::NUMERIC,
                                       ErrorCorrectionLevel::M) == 1);
    REQUIRE(determine_smallest_version(35, EncodingMode::NUMERIC,
                                       ErrorCorrectionLevel::M) == 2);
    REQUIRE(determine_smallest_version(63, EncodingMode::NUMERIC,
                                       ErrorCorrectionLevel::M) == 2);

    REQUIRE_THROWS_AS(determine_smallest_version(9999,
                                                 EncodingMode::ALPHANUMERIC,
                                                 ErrorCorrectionLevel::L),
                      MaxCapacityExceededException);
}

TEST_CASE("Encodes numeric data correctly", "[data_encoding]") {

    REQUIRE(encode_data("8", EncodingMode::NUMERIC, 1) ==
            str_to_bits("0001 0000000001 1000"));
    REQUIRE(encode_data("87", EncodingMode::NUMERIC, 1) ==
            str_to_bits("0001 0000000010 1010111"));
    REQUIRE(encode_data("867", EncodingMode::NUMERIC, 1) ==
            str_to_bits("0001 0000000011 1101100011"));
    REQUIRE(encode_data("8675", EncodingMode::NUMERIC, 1) ==
            str_to_bits("0001 0000000100 1101100011 0101"));
    REQUIRE(encode_data("86754", EncodingMode::NUMERIC, 1) ==
            str_to_bits("0001 0000000101 1101100011 0110110"));
    REQUIRE(encode_data("867541", EncodingMode::NUMERIC, 1) ==
            str_to_bits("0001 0000000110 1101100011 1000011101"));
}

TEST_CASE("Encodes alphanumeric data correctly", "[data_encoding]") {
    REQUIRE(encode_data("H", EncodingMode::ALPHANUMERIC, 1) ==
            str_to_bits("0010 000000001 010001"));
    REQUIRE(encode_data("HE", EncodingMode::ALPHANUMERIC, 1) ==
            str_to_bits("0010 000000010 01100001011"));
    REQUIRE(encode_data("HELLO WORLD", EncodingMode::ALPHANUMERIC, 1) ==
            str_to_bits("0010 000001011 01100001011 01111000110 10001011100"
                        "10110111000 10011010100 001101"));
    REQUIRE(encode_data("HELLO WORLD$", EncodingMode::ALPHANUMERIC, 1) ==
            str_to_bits("0010 000001100 01100001011 01111000110 10001011100"
                        "10110111000 10011010100 01001101110"));
}

TEST_CASE("Encodes byte data correctly", "[data_encoding]") {
    REQUIRE(encode_data("H", EncodingMode::BYTE, 1) ==
            str_to_bits("0100 00000001 " + hex_to_bytes({0x48})));
    REQUIRE(encode_data("Hello, World!", EncodingMode::BYTE, 1) ==
            str_to_bits("0100 00001101 " +
                        hex_to_bytes({0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20,
                                      0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21})));
}
