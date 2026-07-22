#include "qr_code_generator.h"
#include "data_masking/data_mask_resolver.h"
#include "message_encoding/codewords_assembly.h"
#include "message_encoding/data_encoding.h"
#include "module_placement/qr_function_patterns.h"
#include "qr.h"
#include "qr_color_constants.h"
#include "struct/MutableQrCode.h"
#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <vector>

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

void reserve_separators(MutableQrCode& qr, size_t size) {
    qr.draw_rect(RESERVED, 0, 8, 5, 8);
    qr[7, 8] = RESERVED;
    qr[8, 8] = RESERVED;
    qr[8, 7] = RESERVED;
    qr.draw_rect(RESERVED, 8, 0, 8, 5);

    qr.draw_rect(RESERVED, size - 8, 8, size - 1, 8);
    qr.draw_rect(RESERVED, 8, size - 7, 8, size - 1);
}

void reserve_version_information(MutableQrCode& qr, size_t size) {
    qr.draw_rect(RESERVED, size - 11, 0, size - 9, 5);
    qr.draw_rect(RESERVED, 0, size - 11, 5, size - 9);
}

void reserve_finder_patterns(MutableQrCode& qr, size_t size) {
    qr.draw_rect(RESERVED, 0, 0, 8, 8);
    qr.draw_rect(RESERVED, size - 8, 0, size - 1, 8);
    qr.draw_rect(RESERVED, 0, size - 8, 8, size - 1);
}

void reserve_timing_patterns(MutableQrCode& qr, size_t size) {
    qr.draw_rect(RESERVED, 8, 6, size - 8, 6);
    qr.draw_rect(RESERVED, 6, 8, 6, size - 8);
}

void reserve_alignment_patterns(MutableQrCode& qr, size_t size, int version) {
    auto indices = qr::VERSION_TO_ALIGNMENT_PATTERN_LOCATIONS[version];
    for (const auto& x : indices) {
        if (x == 0)
            continue;
        for (const auto& y : indices) {
            if (y == 0)
                continue;
            if (!(x < 10 && y < 10) && !(x > size - 10 && y < 10) &&
                !(x < 10 && y > size - 10)) {
                qr.draw_rect(RESERVED, x - 2, y - 2, x + 2, y + 2);
            }
        }
    }
}

void reserve_area(MutableQrCode& qr, size_t size, int version) {
    reserve_finder_patterns(qr, size);
    reserve_timing_patterns(qr, size);
    reserve_alignment_patterns(qr, size, version);
    reserve_separators(qr, size);
    if (version >= 7) {
        reserve_version_information(qr, size);
    }
}

unsigned char get_bit_color(bool bit) { return bit ? BLACK : WHITE; }

void draw_data_bits(MutableQrCode& qr, const std::vector<bool>& data,
                    size_t size) {
    bool goes_up = true;
    bool place_right = true;
    int row = size - 1;
    int col_x = size - 2;
    int i = 0;

    while (col_x >= 0) {
        if (place_right) {
            auto& module = qr[col_x + 1, row];
            if (module == EMPTY) {
                module = get_bit_color(data[i++]);
            }
        } else {
            auto& module = qr[col_x, row];
            if (module == EMPTY) {
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

void draw_format_string(MutableQrCode& qr, qr::ErrorCorrectionLevel ec_level,
                        unsigned char mask) {
    auto bits = qr::EC_AND_MASK_TO_FORMAT_STRING.at(ec_level)[mask];
    for (int col = 0; col < 6; col++) {
        qr[col, 8] = get_bit_color(bits[col]);
    }
    qr[7, 8] = get_bit_color(bits[6]);
    qr[8, 8] = get_bit_color(bits[7]);
    qr[8, 7] = get_bit_color(bits[8]);
    for (int row = 5; row >= 0; row--) {
        qr[8, row] = get_bit_color(bits[14 - row]);
    }

    for (int row = qr.size() - 1; row >= qr.size() - 7; row--) {
        qr[8, row] = get_bit_color(bits[qr.size() - row - 1]);
    }

    for (int col = qr.size() - 8; col < qr.size(); col++) {
        qr[col, 8] = get_bit_color(bits[col + 15 - qr.size()]);
    }
}

void draw_version_string(MutableQrCode& qr, int version) {
    if (version < 7)
        return;
    auto bits = qr::VERSION_TO_VERSION_STRING[version];
    auto it = bits.rbegin();
    for (int col = 0; col < 6; col++) {
        for (int row = qr.size() - 11; row <= qr.size() - 9; row++) {
            qr[col, row] = get_bit_color(*it++);
        }
    }

    assert(it == bits.rend());
    it = bits.rbegin();

    for (int row = 0; row < 6; row++) {
        for (int col = qr.size() - 11; col <= qr.size() - 9; col++) {
            qr[col, row] = get_bit_color(*it++);
        }
    }
    assert(it == bits.rend());
}

MutableQrCode generate_qr_code(const std::string& data,
                               qr::ErrorCorrectionLevel ec_level,
                               qr::EncodingMode encoding_mode, int version,
                               int mask) {
    auto data_codewords = encode_data(data, encoding_mode, version, ec_level);
    auto codewords = assemble_data_codewords(data_codewords, version, ec_level);
    auto data_bits = serialize_codewords_to_bits_and_pad(codewords, version);
    int modules = version * 4 + 17;

    MutableQrCode qr(modules);
    reserve_area(qr, modules, version);
    draw_data_bits(qr, data_bits, modules);
    if (mask < 0 || mask > 7) {
        auto masking_result = get_best_data_mask(qr, version);
        qr = masking_result.qr;
        mask = masking_result.mask;
    } else {
        qr = mask_data(qr, version, mask);
    }
    draw_format_string(qr, ec_level, mask);
    draw_version_string(qr, version);
    return qr;
}
