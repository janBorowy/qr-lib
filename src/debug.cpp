#include "debug.h"
#include "qr_color_constants.h"
#include <format>
#include <iostream>

void print_matrix(const CImg<unsigned char>& img) {
    for (int row = 0; row < img.height(); row++) {
        std::cout << std::format("{:3}: ", row);
        for (int col = 0; col < img.width(); col++) {
            std::cout << (img(col, row) == WHITE[0] ? "⬜" : "⬛");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}
