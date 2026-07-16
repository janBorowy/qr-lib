#include "../lib/CImg/CImg.h"
#include "../src/data_masking/evaluators.h"
#include "../src/qr_color_constants.h"

#include "catch2/catch_test_macros.hpp"
#include <numeric>

using namespace cimg_library;

constexpr unsigned char W = WHITE[0];
constexpr unsigned char B = BLACK[0];
const CImg<unsigned char> FIRST_RULE_SAMPLE_MATRIX(
    (unsigned char[]){
        W, B, B, B, B, B, W, B, B, W, B, B,  // 1 -> 3
        W, B, B, B, B, B, B, B, W, B, B, B,  // 2 -> 5
        W, B, B, B, B, B, B, B, B, W, B, B,  // 3 -> 6
        W, B, W, W, W, W, W, B, W, B, B, B,  // 4 -> 3
        W, B, B, W, B, W, W, W, W, W, B, W,  // 5 -> 3
        W, B, W, B, W, B, W, B, W, B, W, B,  // 6
        W, W, B, W, B, W, B, W, B, W, B, W,  // 7
        W, B, W, B, W, B, W, B, W, B, W, B,  // 8
        W, W, B, W, B, W, B, W, B, W, W, W,  // 9
        W, B, W, B, W, B, W, B, W, W, W, B,  // 10
        W, W, B, W, B, W, B, W, B, W, B, W,  // 11
        W, B, W, B, W, B, W, B, W, B, W, B}, // 12
                                             // col 0: 10, col 2: 4, col 11: 3
    12, 12);

TEST_CASE("First rule evaluation") {
    REQUIRE(evaluate_first_rule(FIRST_RULE_SAMPLE_MATRIX) == 37);
}

const CImg<unsigned char> SECOND_RULE_SAMPLE_MATRIX(
    (unsigned char[]){W, B, B, B, B, B, W, B, B, W, B, B,  // 1
                      W, B, B, B, B, B, B, B, W, B, B, B,  // 2
                      W, B, B, B, B, B, B, B, B, W, B, B,  // 3
                      W, B, W, W, W, W, W, B, W, B, B, B,  // 4
                      W, B, B, W, B, W, W, W, W, W, B, W,  // 5
                      W, B, W, B, W, B, W, B, W, B, W, B,  // 6
                      W, W, W, W, W, W, B, W, B, W, B, W,  // 7
                      W, B, W, W, W, B, W, B, W, B, W, B,  // 8
                      W, W, W, W, W, W, B, W, B, W, W, W,  // 9
                      W, B, W, B, W, B, W, B, W, W, W, B,  // 10
                      W, W, B, W, B, W, B, W, B, W, B, W,  // 11
                      W, B, W, B, W, B, W, B, W, B, W, B}, // 12

    12, 12);

TEST_CASE("Second rule evaluation") {
    REQUIRE(evaluate_second_rule(SECOND_RULE_SAMPLE_MATRIX) == 57); // 19 * 3
}

const CImg<unsigned char> THIRD_RULE_SAMPLE_MATRIX(
    (unsigned char[]){
        W, W, W, W, B, W, B, B, B, W, B, W, W, W, W, //
        W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, //
        W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, //
        W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, //
        B, W, B, B, B, W, B, W, W, W, W, W, W, W, B, //
        W, W, W, W, W, W, W, W, B, W, W, W, W, W, W, //
        B, W, B, B, B, W, B, W, W, W, W, W, W, W, B, //
        W, W, W, W, W, W, W, W, B, W, W, W, W, W, B, //
        W, W, W, W, W, W, W, W, B, W, W, W, W, W, B, //
        W, W, W, W, B, W, B, B, B, W, B, W, W, W, W, //
        W, W, W, W, W, W, W, W, W, W, W, W, W, W, B, //
        W, W, W, W, W, W, W, W, B, W, W, W, W, W, W, //
        W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, //
        W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, //
        W, W, W, W, W, W, W, W, W, W, W, W, W, W, W  //
    },
    15, 15);

TEST_CASE("Third rule evaluation") {

    REQUIRE(evaluate_third_rule(THIRD_RULE_SAMPLE_MATRIX) == 360); // 9 * 40
}

TEST_CASE("Fourth rule evaluation") {
    const CImg<unsigned char> matrix1(
        (unsigned char[]){
            B, B, B, B, B, B, B, B, B, B, //
            B, B, B, B, B, B, B, B, B, B, //
            B, B, B, B, B, B, B, B, B, B, //
            B, B, B, B, B, B, B, B, B, B, //
            B, B, B, B, B, B, B, B, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W  //
        },
        10, 10);
    REQUIRE(evaluate_fourth_rule(matrix1) == 0);

    const CImg<unsigned char> matrix2(
        (unsigned char[]){
            B, B, B, B, B, B, B, B, B, B, //
            B, B, B, B, B, B, B, B, B, B, //
            B, B, B, B, B, B, B, B, B, B, //
            B, B, B, B, B, B, B, B, B, B, //
            B, B, B, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W  //
        },
        10, 10);
    REQUIRE(evaluate_fourth_rule(matrix2) == 10);

    const CImg<unsigned char> matrix3(
        (unsigned char[]){
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W, //
            W, W, W, W, W, W, W, W, W, W  //
        },
        10, 10);
    REQUIRE(evaluate_fourth_rule(matrix3) == 90);
}

const CImg<unsigned char> SAMPLE_MATRIX(
    (unsigned char[]){
        B, B, B, B, B, B, B, W, B, B, W, W, W, W, B, B, B, B, B, B, B, // 1
        B, W, W, W, W, W, B, W, B, W, W, B, W, W, B, W, W, W, W, W, B, // 2
        B, W, B, B, B, W, B, W, B, W, W, B, B, W, B, W, B, B, B, W, B, // 3
        B, W, B, B, B, W, B, W, B, W, W, W, W, W, B, W, B, B, B, W, B, // 4
        B, W, B, B, B, W, B, W, B, W, B, W, W, W, B, W, B, B, B, W, B, // 5
        B, W, W, W, W, W, B, W, W, W, B, W, W, W, B, W, W, W, W, W, B, // 6
        B, B, B, B, B, B, B, W, B, W, B, W, B, W, B, B, B, B, B, B, B, // 7
        W, W, W, W, W, W, W, W, B, W, W, W, W, W, W, W, W, W, W, W, W, // 8
        W, B, B, W, B, W, B, B, W, W, W, W, B, W, B, W, B, B, B, B, B, // 9
        W, B, W, W, W, W, W, W, B, B, B, B, W, W, W, W, B, W, W, W, B, // 10
        W, W, B, B, W, B, B, B, W, B, B, W, W, W, B, W, B, B, W, W, W, // 11
        W, B, B, W, B, B, W, B, W, W, B, B, W, B, W, B, W, B, B, B, W, // 12
        B, W, W, W, B, W, B, W, B, W, B, B, B, W, B, B, B, W, B, W, B, // 13
        W, W, W, W, W, W, W, W, B, B, W, B, W, W, B, W, W, W, B, W, B, // 14
        B, B, B, B, B, B, B, W, B, W, B, W, W, W, W, B, W, B, B, W, W, // 15
        B, W, W, W, W, W, B, W, W, B, W, B, B, W, B, B, W, B, W, W, W, // 16
        B, W, B, B, B, W, B, W, B, W, B, W, W, W, B, B, B, B, B, B, B, // 17
        B, W, B, B, B, W, B, W, W, B, W, B, W, B, W, B, W, W, W, B, W, // 18
        B, W, B, B, B, W, B, W, B, W, W, W, B, B, B, B, W, B, W, W, B, // 19
        B, W, W, W, W, W, B, W, B, W, B, B, W, B, W, W, W, B, W, B, B, // 20
        B, B, B, B, B, B, B, W, W, W, W, W, B, B, B, B, W, W, W, W, B, // 21
    },
    21, 21);
TEST_CASE("All evaluators against sample qr") {
    int penalty = std::accumulate(
        QR_EVALUATORS.begin(), QR_EVALUATORS.end(), 0,
        [](int p, auto eval) { return p + eval(SAMPLE_MATRIX); });
    REQUIRE(penalty == 350);
}
