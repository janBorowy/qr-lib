#pragma once

#include <string>

#include "../lib/CImg/CImg.h"
#include "qr.h"
#include "qrlib.h"

using cimg_library::CImg;

CImg<unsigned char> generate_qr(const std::string& data,
                                qr::ErrorCorrectionLevel ec_level,
                                qr::EncodingMode encoding_mode, int version,
                                int mask = -1);
