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

TEST_CASE("Encodes data correctly", "[data_encoding]") {
    // REQUIRE(encode_data("HELLO WORLD", EncodingMode::ALPHANUMERIC, 1) ==
    //         std::string{"0010"} + "000001011");

    REQUIRE(encode_data("867", EncodingMode::NUMERIC, 1) ==
            str_to_bits("0001 0000000011 1101100011"));
}
