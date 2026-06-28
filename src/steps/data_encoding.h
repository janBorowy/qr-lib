#include "data_analysis.h"
#include <string>
#include <vector>
constexpr int MODE_INDICATOR_SIZE = 4;

std::vector<bool> encode_data(const std::string& data, EncodingMode mode,
                              unsigned char version);
