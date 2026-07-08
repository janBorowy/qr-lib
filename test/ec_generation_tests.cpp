#include "../src/steps/error_correction/alpha_notation_conversion.h"
#include "../src/steps/error_correction/ec_generation.h"
#include "../src/steps/error_correction/generator_polynomial.h"
#include "../src/steps/error_correction/gf_polynomial.h"
#include "catch2/catch_test_macros.hpp"
#include "test_utils.h"
#include <vector>

TEST_CASE("Generates alpha conversion table", "[alpha_conv]") {
    REQUIRE(ALPHA_TO_INT[0] == 1);
    REQUIRE(ALPHA_TO_INT[1] == 2);
    REQUIRE(ALPHA_TO_INT[2] == 4);
    REQUIRE(ALPHA_TO_INT[8] == 29);
    REQUIRE(ALPHA_TO_INT[255] == 1);

    REQUIRE(INT_TO_ALPHA[1] == 0);
    REQUIRE(INT_TO_ALPHA[2] == 1);
    REQUIRE(INT_TO_ALPHA[3] == 25);
    REQUIRE(INT_TO_ALPHA[4] == 2);
    REQUIRE(INT_TO_ALPHA[5] == 50);
}

TEST_CASE("Multiplies polynomial", "[gf_polynomial]") {
    GFPolynomial first;
    first.set_coef(0, 1);
    first.set_coef(1, 1);
    GFPolynomial second;
    second.set_coef(0, 2);
    second.set_coef(1, 1);
    GFPolynomial result = first.multiply(second);
    REQUIRE(result[0] == 2);
    REQUIRE(result[1] == 3);
    REQUIRE(result[2] == 1);
}

TEST_CASE("Calculates generator polynomial", "[ec_generation]") {
    REQUIRE(get_generator_polynomial(2) ==
            GFPolynomial({{2, 1}, {1, 3}, {0, 2}}));
    REQUIRE(get_generator_polynomial(3) ==
            GFPolynomial({{3, 1}, {2, 7}, {1, 14}, {0, 8}}));
    REQUIRE(
        get_generator_polynomial(5) ==
        GFPolynomial({{5, 1}, {4, 31}, {3, 198}, {2, 63}, {1, 147}, {0, 116}}));
}

TEST_CASE("Calculates error codewords", "[ec_generation]") {
    // HELLO WORLD
    REQUIRE(
        generate_ec_codewords(
            str_to_bits("00100000 01011011 00001011 01111000 11010001 01110010"
                        "11011100 01001101 01000011 01000000 11101100 00010001"
                        "11101100 00010001 11101100 00010001"),
            10) == std::vector<bool>());
}
