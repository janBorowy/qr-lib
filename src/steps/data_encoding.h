#include "data_analysis.h"
#include "parameters.h"
#include <string>
#include <vector>
constexpr int MODE_INDICATOR_SIZE = 4;

// for BYTE mode, input string data MUST be in ISO-8859-1
std::vector<bool> encode_data(const std::string& data, EncodingMode mode,
                              unsigned char version,
                              ErrorCorrectionLevel ec_level);
