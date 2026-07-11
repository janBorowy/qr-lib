#pragma once

#include "../src/steps/qr.h"
#include <string>
#include <vector>

std::vector<qr::Codeword> str_to_codewords(const std::string& str);
