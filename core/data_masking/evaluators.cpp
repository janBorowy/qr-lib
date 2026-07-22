#include "evaluators.h"
#include "../qr_color_constants.h"
#include <algorithm>
#include <cstdlib>

int evaluate_first_rule(const MutableQrCode& qr) {
    int penalty = 0;
    for (int row = 0; row < qr.size(); row++) {
        int current_color = -1;
        int counter = 0;
        for (int col = 0; col < qr.size(); col++) {
            if (qr[col, row] != current_color) {
                penalty += counter >= 5 ? counter - 2 : 0;
                current_color = qr[col, row];
                counter = 0;
            }
            counter++;
        }
        penalty += counter >= 5 ? counter - 2 : 0;
    }

    for (int col = 0; col < qr.size(); col++) {
        int current_color = -1;
        int counter = 0;
        for (int row = 0; row < qr.size(); row++) {
            if (qr[col, row] != current_color) {
                penalty += counter >= 5 ? counter - 2 : 0;
                current_color = qr[col, row];
                counter = 0;
            }
            counter++;
        }
        penalty += counter >= 5 ? counter - 2 : 0;
    }
    return penalty;
}

int evaluate_second_rule(const MutableQrCode& qr) {
    int penalty = 0;
    for (int row = 0; row < qr.size() - 1; row++) {
        for (int col = 0; col < qr.size() - 1; col++) {
            if (qr[col, row] == qr[col + 1, row] &&
                qr[col, row] == qr[col, row + 1] &&
                qr[col, row] == qr[col + 1, row + 1]) {
                penalty += 3;
            }
        }
    }
    return penalty;
}

bool is_equal_horizontal(const MutableQrCode& qr, int x, int y,
                         std::array<unsigned char, 11> pattern) {

    for (const unsigned char& c : pattern) {
        if (qr[x, y] != c) {
            return false;
        }
        x++;
    }
    return true;
}

bool is_equal_vertical(const MutableQrCode& qr, int x, int y,
                       std::array<unsigned char, 11> pattern) {

    for (const unsigned char& c : pattern) {
        if (qr[x, y] != c) {
            return false;
        }
        y++;
    }
    return true;
}

constexpr unsigned char W = WHITE;
constexpr unsigned char B = BLACK;
int evaluate_third_rule(const MutableQrCode& img) {
    int penalty = 0;
    for (int row = 0; row < img.size(); row++) {
        for (int col = 0; col < img.size() - 10; col++) {
            if (is_equal_horizontal(img, col, row,
                                    {B, W, B, B, B, W, B, W, W, W, W}) ||
                is_equal_horizontal(img, col, row,
                                    {W, W, W, W, B, W, B, B, B, W, B})) {
                penalty += 40;
            }
        }
    }

    for (int col = 0; col < img.size(); col++) {
        for (int row = 0; row < img.size() - 10; row++) {
            if (is_equal_vertical(img, col, row,
                                  {B, W, B, B, B, W, B, W, W, W, W}) ||
                is_equal_vertical(img, col, row,
                                  {W, W, W, W, B, W, B, B, B, W, B})) {
                penalty += 40;
            }
        }
    }

    return penalty;
}

int evaluate_fourth_rule(const MutableQrCode& qr) {
    int dark_modules = 0;
    int modules = qr.size() * qr.size();
    for (int row = 0; row < qr.size(); row++) {
        for (int col = 0; col < qr.size(); col++) {
            if (qr[col, row] == BLACK) {
                dark_modules++;
            }
        }
    }

    float dark_percent = static_cast<float>(dark_modules) / modules * 100;
    int prev_five_multiplier = static_cast<int>(dark_percent / 5) * 5;
    int next_five_multiplier = prev_five_multiplier + 5;

    return std::min(abs(prev_five_multiplier - 50) / 5,
                    abs(next_five_multiplier - 50) / 5) *
           10;
}
