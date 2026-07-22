#pragma once

#include <string>

#include "include/qrlib.h"
#include "qr.h"
#include "struct/MutableQrCode.h"

MutableQrCode generate_qr_code(const std::string& data,
                                qr::ErrorCorrectionLevel ec_level,
                                qr::EncodingMode encoding_mode, int version,
                                int mask = -1);
