#include "data_mask_resolver.h"
#include "../qr_color_constants.h"
#include "../qr_function_patterns.h"
#include "evaluators.h"
#include <algorithm>
#include <cstdio>
#include <functional>
#include <numeric>

using DataMask = std::function<bool(int, int)>;

constexpr auto make_masks() {
    return std::array<DataMask, 8>{
        [](int col, int row) { return (row + col) % 2 == 0; },
        [](int col, int row) { return row % 2 == 0; },
        [](int col, int row) { return col % 3 == 0; },
        [](int col, int row) { return (row + col) % 3 == 0; },
        [](int col, int row) { return (row / 2 + col / 3) % 2 == 0; },
        [](int col, int row) { return (row * col) % 2 + (row * col) % 3 == 0; },
        [](int col, int row) {
            return ((row * col) % 2 + (row * col) % 3) % 2 == 0;
        },
        [](int col, int row) {
            return ((row + col) % 2 + (row * col) % 3) % 2 == 0;
        }};
}

const auto DATA_MASKS = make_masks();

CImg<unsigned char> mask_img(const CImg<unsigned char>& img, DataMask mask) {
    CImg<unsigned char> masked_img(img);
    for (int row = 0; row < masked_img.height(); row++) {
        for (int col = 0; col < masked_img.width(); col++) {
            if (mask(col, row) && img(col, row) != RESERVED[0]) {
                masked_img(col, row) =
                    masked_img(col, row) == WHITE[0] ? BLACK[0] : WHITE[0];
            }
        }
    }
    return masked_img;
}

int evalute_img(const CImg<unsigned char>& img) {
    return std::accumulate(
        QR_EVALUATORS.cbegin(), QR_EVALUATORS.cend(), 0,
        [&img](auto p, auto evaluator) { return p + evaluator(img); });
}

// TODO: exclude reserved format and version areas from evaluation

MaskedImage get_best_data_mask(const CImg<unsigned char>& img, int version) {
    std::vector<CImg<unsigned char>> masked_imgs;
    std::vector<int> penalties;
    for (const auto& mask : DATA_MASKS) {
        masked_imgs.push_back(mask_img(img, mask));
    }
    std::for_each(masked_imgs.begin(), masked_imgs.end(),
                  [&version](auto& i) { draw_function_patterns(i, version); });
    CImg img_with_patterns(img);
    draw_function_patterns(img_with_patterns, version);
    for (const auto& masked_img : masked_imgs) {
        penalties.push_back(evalute_img(masked_img));
    }
    auto smallest = std::min_element(penalties.begin(), penalties.end());
    unsigned char smallest_idx = std::distance(penalties.begin(), smallest);
    return {.img = masked_imgs[smallest_idx], .mask = smallest_idx};
}
