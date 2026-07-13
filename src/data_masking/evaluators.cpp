#include "evaluators.h"

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

    for (const char& c : pattern) {
        if (img(x, y) != c) {
            return false;
        }
        x++;
    }
    return true;
}

bool is_equal_vertical(const CImg<unsigned char>& img, int x, int y,
                       std::array<unsigned char, 11> pattern) {

    for (const char& c : pattern) {
        if (img(x, y) != c) {
            return false;
        }
        y++;
    }
    return true;
}

int evaluate_third_rule(const CImg<unsigned char>& img) {
    int penalty = 0;
    for (int row = 0; row < img.height(); row++) {
        for (int col = 0; col < img.width() - 10; col++) {
            if (is_equal_horizontal(img, col, row,
                                    {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0}) ||
                is_equal_horizontal(img, col, row,
                                    {0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1})) {
                penalty += 40;
            }
        }
    }

    for (int col = 0; col < img.width(); col++) {
        for (int row = 0; row < img.height() - 10; row++) {
            if (is_equal_vertical(img, col, row,
                                  {1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0}) ||
                is_equal_vertical(img, col, row,
                                  {0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1})) {
                penalty += 40;
            }
        }
    }

    return penalty;
}
int evaluate_fourth_rule(const CImg<unsigned char>& img) { return 0; }
