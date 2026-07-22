#pragma once
#include "../../lib/CImg/CImg.h"
#include <cstddef>
#include <string>
#include <vector>

class MutableQrCode {
    std::string data;
    std::vector<std::vector<unsigned char>> modules;

  public:
    MutableQrCode(int size);
    MutableQrCode(const cimg_library::CImg<unsigned char>& img);

    cimg_library::CImg<unsigned char> convert() const;
    std::string get_data() const;
    size_t size() const;
    unsigned char operator[](int x, int y) const;
    unsigned char& operator[](int x, int y);
    void draw_rect(unsigned char value, int x0, int y0, int x1, int y1);
};
