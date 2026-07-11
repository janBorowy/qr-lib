#pragma once
#include "qr.h"
#include <vector>

std::vector<qr::Codeword>
assemble_data_codewords(const std::vector<qr::Codeword>& data_codewords,
                        int version, qr::ErrorCorrectionLevel ec_level);
