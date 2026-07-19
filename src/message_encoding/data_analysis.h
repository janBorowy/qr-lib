#pragma once

#include "../qr.h"

#include <cstddef>
#include <string>

qr::EncodingMode analyze_encoding_mode(const std::string& input);
unsigned char determine_smallest_version(size_t input_len, qr::EncodingMode,
                                         qr::ErrorCorrectionLevel);
