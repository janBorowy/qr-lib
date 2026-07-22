#include "../include/QrCode.h"
#include "MutableQrCode.h"
#include <cstddef>
#include <vector>

using namespace qr;

QrCode::QrCode(const MutableQrCode& other)
    : data(other.get_data()),
      modules(std::vector<std::vector<bool>>(
          other.size(), std::vector<bool>(other.size(), false))) {
    for (int row = 0; row < size(); row++) {
        for (int col = 0; col < size(); col++) {
            modules[row][col] = other[row, col] == 255 ? true : false;
        }
    }
}

size_t QrCode::size() const { return modules.size(); }

bool QrCode::operator[](int x, int y) const { return modules[y][x]; }
