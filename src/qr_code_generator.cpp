#include "qr_code_generator.h"
#include "steps/codewords_assembly.h"
#include "steps/data_analysis.h"
#include "steps/data_encoding.h"
#include "steps/qr.h"
#include <bitset>
#include <cstddef>
#include <iostream>
#include <vector>

using namespace cimg_library;

constexpr unsigned char BLACK[] = {0, 0, 0};
constexpr unsigned char EMPTY[] = {64, 0, 0};
constexpr unsigned char RESERVED[] = {196, 0, 0};
constexpr unsigned char WHITE[] = {255, 0, 0};

std::vector<bool>
serialize_codewords_to_bits_and_pad(const std::vector<qr::Codeword> codewords,
                                    int version) {
    std::vector<bool> bits;
    for (const auto& c : codewords) {
        auto binary = std::bitset<8>(c);
        for (int i = 7; i >= 0; i--) {
            bits.push_back(binary[i]);
        }
    }

    for (int i = 0; i < qr::VERSION_TO_REMINDER_BITS[version]; i++) {
        bits.push_back(0);
    }
    return bits;
}

void draw_square(CImg<unsigned char>& img, int x, int y, int s,
                 const unsigned char color[3]) {
    auto x_end = x + s - 1;
    auto y_end = y + s - 1;
    img.draw_line(x, y, x_end, y, color);
    img.draw_line(x, y, x, y_end, color);
    img.draw_line(x_end, y, x_end, y_end, color);
    img.draw_line(x, y_end, x_end, y_end, color);
}

void draw_finder_pattern(CImg<unsigned char>& img, int x, int y) {
    draw_square(img, x, y, 7, BLACK);
    draw_square(img, x + 1, y + 1, 5, WHITE);
    draw_square(img, x + 2, y + 2, 3, BLACK);
    img.atXY(x + 3, y + 3) = 0;
}

void draw_alignment_pattern(CImg<unsigned char>& img, int center_x,
                            int center_y) {
    draw_square(img, center_x - 2, center_y - 2, 5, BLACK);
    draw_square(img, center_x - 1, center_y - 1, 3, WHITE);
    img.atXY(center_x, center_y) = 0;
}

void draw_alignment_patterns(CImg<unsigned char>& img, size_t size,
                             int version) {
    auto indices = qr::VERSION_TO_ALIGNMENT_PATTERN_LOCATIONS[version];
    for (const auto& x : indices) {
        if (x == 0)
            continue;
        for (const auto& y : indices) {
            if (y == 0)
                continue;
            if (!(x < 10 && y < 10) && !(x > size - 10 && y < 10) &&
                !(x < 10 && y > size - 10)) {
                draw_alignment_pattern(img, x, y);
            }
        }
    }
}

void draw_timing_patterns(CImg<unsigned char>& img, size_t size) {
    for (int x = 8; x < size - 8; x++) {
        img.atXY(x, 6) = x % 2 == 0 ? BLACK[0] : WHITE[0];
    }
    for (int y = 8; y < size - 8; y++) {
        img.atXY(6, y) = y % 2 == 0 ? BLACK[0] : WHITE[0];
    }
}

void draw_function_patterns(CImg<unsigned char>& img, size_t size,
                            int version) {
    draw_finder_pattern(img, 0, 0);
    img.draw_line(0, 7, 7, 7, WHITE);
    img.draw_line(7, 0, 7, 7, WHITE);

    draw_finder_pattern(img, size - 7, 0);
    img.draw_line(size - 8, 0, size - 8, 7, WHITE);
    img.draw_line(size - 1, 7, size - 8, 7, WHITE);

    draw_finder_pattern(img, 0, size - 7);
    img.draw_line(0, size - 8, 7, size - 8, WHITE);
    img.draw_line(7, size - 8, 7, size - 1, WHITE);

    draw_alignment_patterns(img, size, version);
    draw_timing_patterns(img, size);
    img.atXY(8, size - 8) = 0;
}

void reserve_area(CImg<unsigned char>& img, size_t size) {
    img.draw_line(0, 8, 5, 8, RESERVED);
    img.atXY(7, 8) = RESERVED[0];
    img.atXY(8, 8) = RESERVED[0];
    img.atXY(8, 7) = RESERVED[0];
    img.draw_line(8, 0, 8, 5, RESERVED);

    img.draw_line(size - 8, 8, size - 1, 8, RESERVED);
    img.draw_line(8, size - 7, 8, size - 1, RESERVED);

    img.draw_rectangle(size - 11, 0, size - 9, 5, RESERVED);
    img.draw_rectangle(0, size - 11, 5, size - 9, RESERVED);
}

unsigned char get_bit_color(bool bit) { return bit ? BLACK[0] : WHITE[0]; }

void draw_data_bits(CImg<unsigned char>& img, const std::vector<bool>& data,
                    size_t size) {
    bool goes_up = true;
    bool place_right = true;
    int row = size - 1;
    int col_x = size - 2;
    int i = 0;

    while (col_x >= 0) {
        if (place_right) {
            auto& module = img.atXY(col_x + 1, row);
            if (module == EMPTY[0]) {
                module = get_bit_color(data[i++]);
            }
        } else {
            auto& module = img.atXY(col_x, row);
            if (module == EMPTY[0]) {
                module = get_bit_color(data[i++]);
            }
            if (goes_up)
                row--;
            else
                row++;
        }
        if (row == size) {
            goes_up = true;
            col_x -= 2;
            row = size - 1;
        } else if (row == -1) {
            goes_up = false;
            col_x -= 2;
            row = 0;
        }
        if (col_x == 5) {
            col_x--;
        }
        place_right = !place_right;
    }
}

CImg<unsigned char> generate_qr(const std::string& data,
                                qr::ErrorCorrectionLevel ec_level) {
    auto encoding_mode = analyze_encoding_mode(data);
    auto version =
        determine_smallest_version(data.size(), encoding_mode, ec_level);
    auto data_codewords = encode_data(data, encoding_mode, version, ec_level);
    auto codewords = assemble_data_codewords(data_codewords, version, ec_level);
    auto data_bits = serialize_codewords_to_bits_and_pad(codewords, version);

    int modules = version * 4 + 17;
    CImg<unsigned char> img(modules, modules, 1, 1, EMPTY[0]);
    draw_function_patterns(img, modules, version);
    reserve_area(img, modules);
    draw_data_bits(img, data_bits, modules);
    return img;
}
