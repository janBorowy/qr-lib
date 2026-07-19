#pragma once

#include "../lib/CImg/CImg.h"
#include "qr.h"
#include <string>

using cimg_library::CImg;

CImg<unsigned char> generate_qr(const std::string& data,
                                qr::ErrorCorrectionLevel ec_level);

CImg<unsigned char> generate_qr(const std::string& data,
                                qr::ErrorCorrectionLevel ec_level, int mask);
