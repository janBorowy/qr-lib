// #include "CLI/App.hpp"
#include "QrCode.h"
#include "qrlib.h"
#include <cstdio>
#include <format>
#include <iostream>

void print_matrix(const qr::QrCode& img) {
    for (int row = 0; row < img.size(); row++) {
        std::cout << std::format("{:3}: ", row);
        for (int col = 0; col < img.size(); col++) {
            std::cout << (img[col, row] ? "⬜" : "⬛");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}

int main() {

    print_matrix(qr::generate_qr("https://www.youtube.com/watch?v=xR-E2pwgFJo",
                                 qr::L, 40));
    return 0;
}
