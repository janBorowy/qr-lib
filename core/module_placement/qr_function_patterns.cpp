#include "qr_function_patterns.h"
#include "../qr.h"
#include "../qr_color_constants.h"

void draw_square(MutableQrCode& qr, int x, int y, int s,
                 const unsigned char color) {
    auto x_end = x + s - 1;
    auto y_end = y + s - 1;
    qr.draw_rect(color, x, y, x_end, y);
    qr.draw_rect(color, x, y, x, y_end);
    qr.draw_rect(color, x_end, y, x_end, y_end);
    qr.draw_rect(color, x, y_end, x_end, y_end);
}

void draw_finder_pattern(MutableQrCode& qr, int x, int y) {
    draw_square(qr, x, y, 7, BLACK);
    draw_square(qr, x + 1, y + 1, 5, WHITE);
    draw_square(qr, x + 2, y + 2, 3, BLACK);
    qr[x + 3, y + 3] = BLACK;
}

void draw_alignment_pattern(MutableQrCode& qr, int center_x, int center_y) {
    draw_square(qr, center_x - 2, center_y - 2, 5, BLACK);
    draw_square(qr, center_x - 1, center_y - 1, 3, WHITE);
    qr[center_x, center_y] = 0;
}

void draw_alignment_patterns(MutableQrCode& qr, size_t size, int version) {
    auto indices = qr::VERSION_TO_ALIGNMENT_PATTERN_LOCATIONS[version];
    for (const auto& x : indices) {
        if (x == 0)
            continue;
        for (const auto& y : indices) {
            if (y == 0)
                continue;
            if (!(x < 10 && y < 10) && !(x > size - 10 && y < 10) &&
                !(x < 10 && y > size - 10)) {
                draw_alignment_pattern(qr, x, y);
            }
        }
    }
}

void draw_timing_patterns(MutableQrCode& img, size_t size) {
    for (int x = 8; x < size - 8; x++) {
        img[x, 6] = x % 2 == 0 ? BLACK : WHITE;
    }
    for (int y = 8; y < size - 8; y++) {
        img[6, y] = y % 2 == 0 ? BLACK : WHITE;
    }
}

void draw_function_patterns(MutableQrCode& img, int version) {
    auto size = version * 4 + 17;
    draw_finder_pattern(img, 0, 0);
    img.draw_rect(WHITE, 0, 7, 7, 7);
    img.draw_rect(WHITE, 7, 0, 7, 7);

    draw_finder_pattern(img, size - 7, 0);
    img.draw_rect(WHITE, size - 8, 0, size - 8, 7);
    img.draw_rect(WHITE, size - 1, 7, size - 8, 7);

    draw_finder_pattern(img, 0, size - 7);
    img.draw_rect(WHITE, 0, size - 8, 7, size - 8);
    img.draw_rect(WHITE, 7, size - 8, 7, size - 1);

    draw_alignment_patterns(img, size, version);
    draw_timing_patterns(img, size);
    img[8, size - 8] = 0;
}
