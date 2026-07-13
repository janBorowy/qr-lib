#include "qr_function_patterns.h"
#include "qr_color_constants.h"
#include "steps/qr.h"

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

void draw_function_patterns(CImg<unsigned char>& img, int version) {
    auto size = version * 4 + 17;
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
