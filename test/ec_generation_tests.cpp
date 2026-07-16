#include "../src/steps/error_correction/alpha_notation_conversion.h"
#include "../src/steps/error_correction/ec_generation.h"
#include "../src/steps/error_correction/generator_polynomial.h"
#include "../src/steps/error_correction/gf_polynomial.h"
#include "catch2/catch_test_macros.hpp"

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

TEST_CASE("Calculates error codewords", "[ec_generation]") { // HELLO WORLD
    REQUIRE(generate_ec_codewords({32, 91, 11, 120, 209, 114, 220, 77, 67, 64,
                                   236, 17, 236, 17, 236, 17},
                                  10) ==
            std::vector<unsigned char>{196, 35, 39, 119, 235, 215, 231, 226, 93,
                                       23});

    REQUIRE(generate_ec_codewords(
                {67, 85, 70, 134, 87, 38, 85, 194, 119, 50, 6, 18, 6, 103, 38},
                18) == std::vector<unsigned char>{213, 199, 11, 45, 115, 247,
                                                  241, 223, 229, 248, 154, 117,
                                                  154, 111, 86, 161, 111, 39});

    REQUIRE(generate_ec_codewords(
                {67, 85, 70, 134, 87, 38, 85, 194, 119, 50, 6, 18, 6, 103, 38},
                18) == std::vector<unsigned char>{213, 199, 11, 45, 115, 247,
                                                  241, 223, 229, 248, 154, 117,
                                                  154, 111, 86, 161, 111, 39});

    REQUIRE(generate_ec_codewords({246, 246, 66, 7, 118, 134, 242, 7, 38, 86,
                                   22, 198, 199, 146, 6},
                                  18) ==
            std::vector<unsigned char>{87, 204, 96, 60, 202, 182, 124, 157, 200,
                                       134, 27, 129, 209, 17, 163, 163, 120,
                                       133});

    REQUIRE(generate_ec_codewords({182, 230, 247, 119, 50, 7, 118, 134, 87, 38,
                                   82, 6, 134, 151, 50, 7},
                                  18) ==
            std::vector<unsigned char>{148, 116, 177, 212, 76, 133, 75, 242,
                                       238, 76, 195, 230, 189, 10, 108, 240,
                                       192, 141});

    REQUIRE(generate_ec_codewords({70, 247, 118, 86, 194, 6, 151, 50, 224, 236,
                                   17, 236, 17, 236, 17, 236},
                                  18) ==
            std::vector<unsigned char>{140, 100, 250, 247, 108, 131, 37, 104,
                                       253, 113, 111, 235, 197, 83, 6, 205, 89,
                                       74});

    // "Hello"
    REQUIRE(generate_ec_codewords({64, 84, 134, 86, 0xc6, 0xc6, 0xf0, 0x11,
                                   0xec, 0x11, 0xec, 0x11, 0xec},
                                  13) ==
            std::vector<unsigned char>{211, 79, 219, 65, 102, 100, 29, 1, 97,
                                       54, 192, 4, 66});
}

