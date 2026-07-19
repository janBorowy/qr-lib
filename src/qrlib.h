#pragma once

#include "../lib/CImg/CImg.h"
#include <string>

using cimg_library::CImg;

namespace qr {
enum ErrorCorrectionLevel { L, M, Q, H };
}

CImg<unsigned char> generate_qr(const std::string& data,
                                qr::ErrorCorrectionLevel ec_level = qr::Q,
                                int version = 0, int mask = -1,
                                bool add_quiet_zone = true);
