#include "evaluators.h"
#include "../qr_color_constants.h"
#include <algorithm>
#include <cstdlib>

int evaluate_first_rule(const CImg<unsigned char>& img) {
    int penalty = 0;
    for (int row = 0; row < img.height(); row++) {
        int current_color = -1;
        int counter = 0;
        for (int col = 0; col < img.width(); col++) {
            if (img.atXY(col, row) != current_color) {
                penalty += counter >= 5 ? counter - 2 : 0;
                current_color = img.atXY(col, row);
                counter = 0;
            }
            counter++;
        }
        penalty += counter >= 5 ? counter - 2 : 0;
    }

    for (int col = 0; col < img.width(); col++) {
        int current_color = -1;
        int counter = 0;
        for (int row = 0; row < img.height(); row++) {
            if (img.atXY(col, row) != current_color) {
                penalty += counter >= 5 ? counter - 2 : 0;
                current_color = img.atXY(col, row);
                counter = 0;
            }
            counter++;
        }
        penalty += counter >= 5 ? counter - 2 : 0;
    }
    return penalty;
}

int evaluate_second_rule(const CImg<unsigned char>& img) {
    int penalty = 0;
    for (int row = 0; row < img.height() - 1; row++) {
        for (int col = 0; col < img.width() - 1; col++) {
            if (img.atXY(row, col) == img.atXY(row, col + 1) &&
                img.atXY(row, col) == img.atXY(row + 1, col) &&
                img.atXY(row, col) == img.atXY(row + 1, col + 1)) {
                penalty += 3;
            }
        }
    }
    return penalty;
}

bool is_equal_horizontal(const CImg<unsigned char>& img, int x, int y,
                         std::array<unsigned char, 11> pattern) {

    for (const unsigned char& c : pattern) {
        if (img(x, y) != c) {
            return false;
        }
        x++;
    }
    return true;
}

bool is_equal_vertical(const CImg<unsigned char>& img, int x, int y,
                       std::array<unsigned char, 11> pattern) {

    for (const unsigned char& c : pattern) {
        if (img(x, y) != c) {
            return false;
        }
        y++;
    }
    return true;
}

constexpr unsigned char W = WHITE[0];
constexpr unsigned char B = BLACK[0];
int evaluate_third_rule(const CImg<unsigned char>& img) {
    int penalty = 0;
    for (int row = 0; row < img.height(); row++) {
        for (int col = 0; col < img.width() - 10; col++) {
            if (is_equal_horizontal(img, col, row,
                                    {B, W, B, B, B, W, B, W, W, W, W}) ||
                is_equal_horizontal(img, col, row,
                                    {W, W, W, W, B, W, B, B, B, W, B})) {
                penalty += 40;
            }
        }
    }

    for (int col = 0; col < img.width(); col++) {
        for (int row = 0; row < img.height() - 10; row++) {
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

int evaluate_fourth_rule(const CImg<unsigned char>& img) {
    int dark_modules = 0;
    int modules = img.height() * img.width();
    for (int row = 0; row < img.height(); row++) {
        for (int col = 0; col < img.width(); col++) {
            if (img(col, row) == BLACK[0]) {
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
