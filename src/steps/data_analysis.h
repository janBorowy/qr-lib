#pragma once

#include <string>

enum class ENCODING_MODE { NUMERIC, ALPHANUMERIC, BYTE };

ENCODING_MODE analyze_encoding_mode(std::string& input);
