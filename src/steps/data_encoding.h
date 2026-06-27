#include "data_analysis.h"
#include <string>
constexpr int MODE_INDICATOR_SIZE = 4;

std::string encode_data(const std::string& data, EncodingMode mode,
                        unsigned char version);
