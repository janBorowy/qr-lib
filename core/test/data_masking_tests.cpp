#include "../data_masking/data_mask_resolver.h"
#include "../qr_color_constants.h"
#include "catch2/catch_test_macros.hpp"

constexpr unsigned char R = RESERVED;
constexpr unsigned char W = WHITE;
constexpr unsigned char B = BLACK;

const MutableQrCode SAMPLE_QR_MATRIX(
    std::vector<unsigned char>{
        W, B, W, B, W, B, W, W, W, B, B, W, B, W, W, B, W, B, W, B, W, // 1
        B, B, W, B, W, B, B, B, B, B, W, W, W, B, B, B, W, B, W, B, B, // 2
        W, W, W, B, W, W, W, W, W, W, B, B, W, W, W, W, W, B, W, W, W, // 3
        B, B, B, W, B, B, B, B, B, B, W, B, W, B, B, B, B, W, B, B, B, // 4
        W, W, W, B, W, W, W, W, W, W, W, W, B, W, W, W, W, B, W, W, W, // 5
        B, B, W, B, W, B, B, B, W, B, B, B, W, B, B, B, W, B, W, B, B, // 6
        W, B, W, B, W, B, W, W, W, W, W, W, W, W, W, B, W, B, W, B, W, // 7
        W, B, W, B, W, B, W, B, B, B, W, B, W, B, W, B, W, B, W, B, W, // 8
        B, B, W, W, W, W, W, B, B, W, B, W, W, W, W, W, W, B, W, B, W, // 9
        W, W, W, B, W, B, W, B, B, W, B, W, W, B, W, B, B, B, W, B, B, // 10
        B, W, W, B, B, B, W, B, B, B, W, W, B, W, W, W, W, B, B, W, B, // 11
        W, W, B, B, B, W, W, W, W, B, B, W, W, W, W, W, W, W, B, W, W, // 12
        W, W, B, W, W, W, W, W, W, W, W, B, W, W, W, B, W, W, W, W, W, // 13
        W, B, W, B, W, B, W, B, B, W, W, W, W, B, B, B, W, B, B, B, B, // 14
        W, B, W, B, W, B, W, W, W, W, W, W, B, W, B, B, B, B, W, W, B, // 15
        B, B, W, B, W, B, B, B, W, W, W, W, B, B, B, W, W, W, W, B, W, // 16
        W, W, W, B, W, W, W, W, W, W, W, W, B, W, W, B, W, B, W, B, W, // 17
        B, B, B, W, B, B, B, B, W, W, W, W, W, W, W, W, W, B, W, W, W, // 18
        W, W, W, B, W, W, W, W, W, W, B, W, W, B, W, B, B, B, B, W, W, // 19
        B, B, W, B, W, B, B, B, B, B, B, W, W, W, W, B, W, W, W, W, B, // 20
        W, B, W, B, W, B, W, W, B, W, B, W, W, B, W, B, B, W, B, W, W, // 21
    },
    21);

TEST_CASE("Resolve best data mask") {

    REQUIRE(get_best_data_mask(SAMPLE_QR_MATRIX, 1).mask == 6);
}
