#include "qr_code_generator.h"
#include "data_masking/data_mask_resolver.h"
#include "qr_color_constants.h"
#include "qr_function_patterns.h"
#include "steps/codewords_assembly.h"
#include "steps/data_analysis.h"
#include "steps/data_encoding.h"
#include "steps/qr.h"
#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <vector>

using namespace cimg_library;

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

void reserve_separators(CImg<unsigned char>& img, size_t size) {
    img.draw_line(0, 8, 5, 8, RESERVED);
    img.atXY(7, 8) = RESERVED[0];
    img.atXY(8, 8) = RESERVED[0];
    img.atXY(8, 7) = RESERVED[0];
    img.draw_line(8, 0, 8, 5, RESERVED);

    img.draw_line(size - 8, 8, size - 1, 8, RESERVED);
    img.draw_line(8, size - 7, 8, size - 1, RESERVED);
}

void reserve_version_information(CImg<unsigned char>& img, size_t size) {
    img.draw_rectangle(size - 11, 0, size - 9, 5, RESERVED);
    img.draw_rectangle(0, size - 11, 5, size - 9, RESERVED);
}

void reserve_finder_patterns(CImg<unsigned char>& img, size_t size) {
    img.draw_rectangle(0, 0, 8, 8, RESERVED);
    img.draw_rectangle(size - 8, 0, size - 1, 8, RESERVED);
    img.draw_rectangle(0, size - 8, 8, size - 1, RESERVED);
}

void reserve_timing_patterns(CImg<unsigned char>& img, size_t size) {
    img.draw_line(8, 6, size - 8, 6, RESERVED);
    img.draw_line(6, 8, 6, size - 8, RESERVED);
}

void reserve_alignment_patterns(CImg<unsigned char>& img, size_t size,
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
                img.draw_rectangle(x - 2, y - 2, x + 2, y + 2, RESERVED);
            }
        }
    }
}

void reserve_area(CImg<unsigned char>& img, size_t size, int version) {
    reserve_finder_patterns(img, size);
    reserve_timing_patterns(img, size);
    reserve_alignment_patterns(img, size, version);
    reserve_separators(img, size);
    if (version >= 7) {
        reserve_version_information(img, size);
    }
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

    assert(i == data.size());
}

void draw_format_string(CImg<unsigned char>& img,
                        qr::ErrorCorrectionLevel ec_level, unsigned char mask) {
    auto bits = qr::EC_AND_MASK_TO_FORMAT_STRING.at(ec_level)[mask];
    for (int col = 0; col < 6; col++) {
        img(col, 8) = get_bit_color(bits[col]);
    }
    img(7, 8) = get_bit_color(bits[6]);
    img(8, 8) = get_bit_color(bits[7]);
    img(8, 7) = get_bit_color(bits[8]);
    for (int row = 5; row >= 0; row--) {
        img(8, row) = get_bit_color(bits[14 - row]);
    }

    for (int row = img.height() - 1; row >= img.height() - 7; row--) {
        img(8, row) = get_bit_color(bits[img.height() - row - 1]);
    }

    for (int col = img.width() - 8; col < img.width(); col++) {
        img(col, 8) = get_bit_color(bits[col + 15 - img.width()]);
    }
}

void draw_version_string(CImg<unsigned char>& img, int version) {
    if (version < 7)
        return;
    auto bits = qr::VERSION_TO_VERSION_STRING[version];
    auto it = bits.rbegin();
    for (int col = 0; col < 6; col++) {
        for (int row = img.height() - 11; row <= img.height() - 9; row++) {
            img(col, row) = get_bit_color(*it++);
        }
    }

    assert(it == bits.rend());
    it = bits.rbegin();

    for (int row = 0; row < 6; row++) {
        for (int col = img.width() - 11; col <= img.width() - 9; col++) {
            img(col, row) = get_bit_color(*it++);
        }
    }
    assert(it == bits.rend());
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
    reserve_area(img, modules, version);
    draw_data_bits(img, data_bits, modules);
    auto masked_img = get_best_data_mask(img, version);
    draw_format_string(masked_img.img, ec_level, masked_img.mask);
    draw_version_string(masked_img.img, version);
    return masked_img.img;
}
