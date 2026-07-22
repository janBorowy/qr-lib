#include "data_mask_resolver.h"
#include "../module_placement/qr_function_patterns.h"
#include "../qr_color_constants.h"
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

MutableQrCode mask_img(const MutableQrCode& qr, DataMask mask) {
    MutableQrCode masked_qr(qr);
    for (int row = 0; row < masked_qr.size(); row++) {
        for (int col = 0; col < masked_qr.size(); col++) {
            if (mask(col, row) && qr[col, row] != RESERVED[0]) {
                masked_qr[col, row] =
                    masked_qr[col, row] == WHITE[0] ? BLACK[0] : WHITE[0];
            }
        }
    }
    return masked_qr;
}

MutableQrCode make_reserved_areas_white(const MutableQrCode& qr) {
    auto result = qr;
    for (int row = 0; row < qr.size(); row++) {
        for (int col = 0; col < qr.size(); col++) {
            if (result[col, row] == RESERVED[0]) {
                result[col, row] = WHITE[0];
            }
        }
    }
    return result;
}

int evalute_img(const MutableQrCode& qr) {
    auto qr_for_evaluation = make_reserved_areas_white(qr);
    return std::accumulate(QR_EVALUATORS.cbegin(), QR_EVALUATORS.cend(), 0,
                           [&qr_for_evaluation](auto p, auto evaluator) {
                               return p +
                                      evaluator(qr_for_evaluation);
                           });
}

MaskedImage get_best_data_mask(const MutableQrCode& qr, int version) {
    std::vector<MutableQrCode> masked_qrs;
    std::vector<int> penalties;
    for (const auto& mask : DATA_MASKS) {
        masked_qrs.push_back(mask_img(qr, mask));
    }
    std::for_each(masked_qrs.begin(), masked_qrs.end(),
                  [&version](auto& i) { draw_function_patterns(i, version); });
    for (const auto& masked_img : masked_qrs) {
        penalties.push_back(evalute_img(masked_img));
    }
    auto smallest = std::min_element(penalties.begin(), penalties.end());
    unsigned char smallest_idx = std::distance(penalties.begin(), smallest);
    return {.qr = masked_qrs[smallest_idx], .mask = smallest_idx};
}

MutableQrCode mask_data(const MutableQrCode& qr, int version, int mask) {
    MutableQrCode result(mask_img(qr, DATA_MASKS[mask]));
    draw_function_patterns(result, version);
    return result;
}
