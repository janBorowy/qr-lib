#pragma once

#include "QrCode.h"
#include <string>

namespace qr {
enum ErrorCorrectionLevel { L, M, Q, H };
qr::QrCode generate_qr(const std::string& data,
                       qr::ErrorCorrectionLevel ec_level = qr::Q,
                       int version = 0, int mask = -1,
                       bool add_quiet_zone = true);
} // namespace qr
