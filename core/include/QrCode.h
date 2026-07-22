#pragma once
#include "../struct/MutableQrCode.h"
#include <cstddef>
#include <string>
#include <vector>

namespace qr {

class QrCode {
    std::string data;
    std::vector<std::vector<bool>> modules;

    QrCode(const QrCode& other) = delete;
    QrCode& operator=(const QrCode& other) = delete;
    QrCode(const QrCode&& other) = delete;
    QrCode& operator=(const QrCode&& other) = delete;

  public:
    QrCode(const MutableQrCode& qr);
    size_t size() const;
    bool operator[](int x, int y) const;
};

} // namespace qr
