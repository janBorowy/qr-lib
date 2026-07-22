#include "MutableQrCode.h"
#include "../qr_color_constants.h"
#include <string>
#include <vector>

MutableQrCode::MutableQrCode(int size)
    : modules(std::vector<std::vector<unsigned char>>(
          size, std::vector<unsigned char>(size, EMPTY[0]))) {}

unsigned char MutableQrCode::operator[](int x, int y) const {
    return modules[y][x];
}

unsigned char& MutableQrCode::operator[](int x, int y) { return modules[y][x]; }

void MutableQrCode::draw_rect(unsigned char value, int x0, int y0, int x1,
                              int y1) {
    auto left = std::min(x0, x1);
    auto right = std::max(x0, x1);
    auto up = std::min(y0, y1);
    auto down = std::max(y0, y1);
    for (int row = up; row <= down; row++) {
        for (int col = left; col <= right; col++) {
            modules[row][col] = value;
        }
    }
}

std::string MutableQrCode::get_data() const { return data; }

size_t MutableQrCode::size() const { return modules.size(); }

// TODO: remove this
cimg_library::CImg<unsigned char> MutableQrCode::convert() const {
    cimg_library::CImg<unsigned char> result(size(), size());
    for (int row = 0; row < size(); row++) {
        for (int col = 0; col < size(); col++) {
            result(col, row) = modules[row][col];
        }
    }
    return result;
}

MutableQrCode::MutableQrCode(const cimg_library::CImg<unsigned char>& img)
    : modules(std::vector<std::vector<unsigned char>>(
          img.height(), std::vector<unsigned char>(img.width(), 0))) {
    for (int row = 0; row < img.height(); row++) {
        for (int col = 0; col < img.width(); col++) {
            modules[row][col] = img(col, row);
        }
    }
}
