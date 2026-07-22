#pragma once
#include <cstddef>
#include <string>
#include <vector>

class MutableQrCode {
    std::string data;
    std::vector<std::vector<unsigned char>> modules;

  public:
    MutableQrCode(int size);
    MutableQrCode(const std::vector<unsigned char>& modules, size_t size);

    std::string get_data() const;
    size_t size() const;
    unsigned char operator[](int x, int y) const;
    unsigned char& operator[](int x, int y);
    void draw_rect(unsigned char value, int x0, int y0, int x1, int y1);
};
