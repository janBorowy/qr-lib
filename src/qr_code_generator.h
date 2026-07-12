#pragma once

#include "../lib/CImg/CImg.h"
#include "steps/qr.h"
#include <string>

cimg_library::CImg<unsigned char>
generate_qr(const std::string& data, qr::ErrorCorrectionLevel ec_level);
