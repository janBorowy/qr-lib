#pragma once

#include "parameters.h"

#include <cstddef>
#include <string>

enum class EncodingMode { NUMERIC, ALPHANUMERIC, BYTE };

EncodingMode analyze_encoding_mode(const std::string& input);
unsigned char determine_smallest_version(size_t input_len, EncodingMode,
                                         ErrorCorrectionLevel);
